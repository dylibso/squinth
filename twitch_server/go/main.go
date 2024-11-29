package main

import (
	"fmt"
	"log"
	"net/http"
	"os"
	"strings"
	"time"

	"strings"

	"github.com/gempir/go-twitch-irc/v4"
	"github.com/joho/godotenv"
)

// TODO: update for the kind of commands that twitch users will send
func twitchWorker(moduleQueue chan []byte) {
	// or client := twitch.NewAnonymousClient() for an anonymous user (no write capabilities)
	user := strings.TrimSpace(os.Getenv("TWITCH_USER"))
	key := strings.TrimSpace(os.Getenv("TWITCH_OAUTH"))
	channel := strings.TrimSpace(os.Getenv("CHANNEL"))

	var xtp_extension string = strings.TrimSpace(os.Getenv("XTP_EXTENSION_ID"))
	var xtp_token string = strings.TrimSpace(os.Getenv("XTP_TOKEN"))

	client := twitch.NewClient(user, key)

	client.OnPrivateMessage(func(message twitch.PrivateMessage) {
		fmt.Println(message.Message)
		if message.Message[0] == '!' {
			plugin_name := message.Message[1:]

			if strings.Contains(plugin_name, " ") {
				// TODO: send error msg back thru twitch chat
				client.Reply(message.Channel, message.ID, "Send a message that follows the format \"!<plugin_name>\"")
				return
			}

			// check if plugin exists
			// TODO: I started on a system that would make  a local copy of the registered plugins, but it's not essential
			// 		would lend itself to more informative error messaging though. And lend itself to showing the users what is available

			plugin, err := getWasmByPluginName(plugin_name, xtp_extension, xtp_token)
			if err != nil { // TODO: consolidate magic number
				client.Reply(message.Channel, message.ID, fmt.Sprintf("Error while trying to fetch plugin: %s", err))
				return
			}

			// TODO: consolidate magic number
			if len(moduleQueue) == 16 {
				client.Reply(message.Channel, message.ID, "The command buffer is full, blocking until another module is consumed")
			}
			moduleQueue <- plugin
			client.Reply(message.Channel, message.ID, fmt.Sprintf("Successfully enqueued: %s", plugin))
		}
	})

	client.OnUserJoinMessage(func(message twitch.UserJoinMessage) {
		client.Say(message.Channel, "WELCOME: "+message.User)
	})

	fmt.Println("Joining ", channel, "...")
	client.Join(channel)

	err := client.Connect()
	if err != nil {
		panic(err)
	}
}

// --------------------------------------------------------------------------------------

func fakeTwitchWorker(moduleQueue chan []byte, pluginName string) {
	var xtp_extension string = strings.TrimSpace(os.Getenv("XTP_EXTENSION_ID"))
	var xtp_token string = strings.TrimSpace(os.Getenv("XTP_TOKEN"))

	// Every 5 seconds fetch the same plugin and add it to the queue to simulate twitch user activity
	for {
		time.Sleep(time.Second * 5)
		fmt.Printf("\nfake twitch worker fetching: %s\n", pluginName)

		mod, err := getWasmByPluginName(
			pluginName,
			xtp_extension,
			xtp_token,
		)

		if mod == nil || err != nil {
			fmt.Println("Failed to fetch plugin")
			if err != nil {
				fmt.Printf("\nError: %s\n", err)
			}
			continue
		} else if len(moduleQueue) >= 16 {
			fmt.Println("Queue is at capacity, blocking until slot opens...")
		}

		moduleQueue <- mod

		fmt.Printf("\nSuccessfully Queued: %s\n", pluginName)
	}
}

// func jsonServerWorker(chatUpdates chan string) {
// 	http.HandleFunc("/twitch-queue", func(respWriter http.ResponseWriter, req *http.Request) {
// 		data := make(map[string]string)

// 		// loop until all messages have been flushed, then respond with the json data
// 		for {
// 			select {
// 			case msg := <-chatUpdates:
// 				fmt.Println("Flushed message: ", msg)
// 				cmdParts := strings.Split(msg, " ")
// 				data[cmdParts[0]] = cmdParts[1]
// 			default:
// 				// No more messages in the channel
// 				// Convert the data structure to JSON
// 				jsonData, err := json.Marshal(data)
// 				if err != nil {
// 					http.Error(respWriter, err.Error(), http.StatusInternalServerError)
// 					return
// 				}

// 				// Set the content type to application/json
// 				respWriter.Header().Set("Content-Type", "application/json")

// 				// Write the JSON data to the response
// 				respWriter.Write(jsonData)
// 				return
// 			}
// 		}

// 	})

// 	fmt.Println("Server is running at 0.0.0.0:5309")
// 	log.Fatal(http.ListenAndServe("0.0.0.0:5309", nil))
// }

func wasmModServerWorker(modQueue chan []byte) {
	// TODO: Endpoint that will return the NAME of most recently transferred module
	//   not yet sure how to keep track of that yet in way that doesn't require me to keep 2 lists
	//   in sync
	//   This will be used to update the label that is displayed on the vcv module

	http.HandleFunc("/module-queue", func(respWriter http.ResponseWriter, req *http.Request) {
		fmt.Println("Request For Wasm Module Received")
		select {
		case module := <-modQueue:
			fmt.Println("Module Pulled from the Queue, sending module...")
			respWriter.Header().Set("Content-Type", "application/wasm")
			respWriter.Header().Set("Content-Length", fmt.Sprintf("%d", len(module)))
			respWriter.Write(module)
		default:
			fmt.Println("Request Made, but no modules are currently in the queue")
			http.Error(respWriter, "No modules in queue", http.StatusNotFound)
			return
		}
	})

	fmt.Println("wasmModServerWorker Server is running at 0.0.0.0:5309")
	log.Fatal(http.ListenAndServe("0.0.0.0:5309", nil))
}

func main() {
	// loads the twitch user and oauth from .env file
	err := godotenv.Load(".env")
	if err != nil {
		panic(err)
	}

	// queue of syth modules
	moduleQueue := make(chan []byte, 16)

	go fakeTwitchWorker(moduleQueue, "zig_template")
	go wasmModServerWorker(moduleQueue)

	select {}
}

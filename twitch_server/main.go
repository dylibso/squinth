package main

import (
	"encoding/json"
	"fmt"
	"log"
	"net/http"
	"os"
	"strings"
	"time"

	"github.com/gempir/go-twitch-irc/v4"
	"github.com/joho/godotenv"
	"golang.org/x/exp/maps"
)

func twitchWorker(
	moduleQueue chan []byte,
	nameQueue chan string,
	xtpExtension string,
	xtpToken string,
) {
	oauth := strings.TrimSpace(os.Getenv("TWITCH_OAUTH"))
	channel := strings.TrimSpace(os.Getenv("CHANNEL"))

	plugin_list, err := fetchPluginList(xtpExtension, xtpToken)
	initialListMsg := fmt.Sprintf("Please try one of the commands below:\n\n#%s",
		strings.Join(maps.Keys(plugin_list), "\n#"))
	if err != nil {
		fmt.Println("Failed to complete initial retrieval of plugin list: ", err)
	} else {
		fmt.Println(initialListMsg)
	}

	client := twitch.NewClient(channel, oauth)

	client.Say(channel, initialListMsg)

	client.OnPrivateMessage(func(message twitch.PrivateMessage) {
		fmt.Println("Received message:", message.Message)
		if strings.TrimSpace(message.Message)[0] == '#' {
			plugin_name := strings.TrimSpace(message.Message[1:])

			if strings.Contains(plugin_name, " ") {
				client.Reply(message.Channel, message.ID, "To load a new plugin, send a message that follows the format \"#<plugin_name>\"")
				return
			}

			pluginInfo, exists := plugin_list[plugin_name]

			if !exists {
				responseString := fmt.Sprintf("Plugin >%s< was not found int the plugin list. Please try one of these commands:\n#%s",
					plugin_name, strings.Join(maps.Keys(plugin_list), "\n#"))
				client.Reply(
					message.Channel,
					message.ID,
					responseString,
				)
				return
			}

			wasmFile, err := getWasmFile(pluginInfo.ContentAddress, xtpToken)
			if err != nil {
				fmt.Printf("Error while trying to fetch plugin: %s", err)
				client.Reply(message.Channel, message.ID,
					fmt.Sprintf("An Error occurred while trying to fetch >%s< :(", plugin_name),
				)
				return
			}

			if len(moduleQueue) == 16 {
				client.Reply(message.Channel, message.ID, "The queue is full, blocking until another module is consumed")
			}
			moduleQueue <- wasmFile
			nameQueue <- plugin_name
			client.Reply(message.Channel, message.ID, fmt.Sprintf("Successfully enqueued: %s", plugin_name))
		}
	})

	client.OnUserJoinMessage(func(message twitch.UserJoinMessage) {
		client.Say(message.Channel, "WELCOME: "+message.User+"!")
	})

	fmt.Println("Joining ", channel, "...")
	client.Join(channel)

	err = client.Connect()
	if err != nil {
		panic(err)
	}
}

// --------------------------------------------------------------------------------------

func fakeTwitchWorker(
	moduleQueue chan []byte,
	pluginName string,
	xtpExtension string,
	xtpToken string,
) {
	// Every 5 seconds fetch the same plugin and add it to the queue to simulate twitch user activity
	for {
		time.Sleep(time.Second * 5)
		fmt.Printf("\nfake twitch worker fetching: %s\n", pluginName)

		mod, err := getWasmByPluginName(
			pluginName,
			xtpExtension,
			xtpToken,
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

func wasmModServerWorker(modQueue chan []byte, nameQueue chan string) {
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

	http.HandleFunc("/name-queue", func(respWriter http.ResponseWriter, req *http.Request) {
		// check the name queue for the name of the module and set that as a header if you have one
		data := make(map[string]string)

		select {
		case modName := <-nameQueue:
			data["name"] = modName
		default:
			data["name"] = "user_module"
		}
		// Convert the data structure to JSON
		jsonData, err := json.Marshal(data)
		if err != nil {
			http.Error(respWriter, err.Error(), http.StatusInternalServerError)
			return
		}

		// Set the content type to application/json
		respWriter.Header().Set("Content-Type", "application/json")

		// Write the JSON data to the response
		respWriter.Write(jsonData)

		return
	})

	fmt.Println("wasmModServerWorker Server is running at 0.0.0.0:5310")
	log.Fatal(http.ListenAndServe("0.0.0.0:5310", nil))
}

func main() {
	// loads the twitch user and oauth from .env file
	err := godotenv.Load(".env")
	if err != nil {
		panic(err)
	}

	var xtp_extension string = strings.TrimSpace(os.Getenv("XTP_EXTENSION_ID"))
	var xtp_token string = strings.TrimSpace(os.Getenv("XTP_TOKEN"))

	// queue of synth modules
	// Two queues is not ideal and can be avoided, but keeps things simple for now
	moduleQueue := make(chan []byte, 16)
	nameQueue := make(chan string, 16)
	go twitchWorker(moduleQueue, nameQueue, xtp_extension, xtp_token)
	go wasmModServerWorker(moduleQueue, nameQueue)

	select {}
}

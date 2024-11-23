// change this implementation to include guest commands
async function getCommands() {
  return await xtpClient.listAvailablePlugins(
    EXT_NAME,
    GUEST_KEY, // TODO: I think I will need some sort of "admin" way of listing the available plugins.
  )
}

async function get_wasm_file(content_address) {
  const url = 'https://xtp.dylibso.com/api/v1/' + content_address;
  const xtp_token = process.env.XTP_TOKEN.trim();
  const options = {method: 'GET', headers: {Authorization: 'Bearer '+xtp_token}};
  
  try {
    const response = await fetch(url, options);
    const data = await response.json();
    console.log("SUCCESS");
  } catch (error) {
    console.error(error);
  }  
}

async function get_wasm_by_plugin_name(plugin_name) {
  // Fetch all artifacts at this extension point TODO: Will this list the artifacts associated with multiple guest keys?
  const extension_id = "ext_01j9hn0bshfh2shvdxk9xz7zys";
  const xtp_token = process.env.XTP_TOKEN.trim();
  const url = 'https://xtp.dylibso.com/api/v1/extension-points/'+extension_id+'/bindings/';
  const options = {method: 'GET', headers: {Authorization: 'Bearer '+xtp_token}};
  
  try {
    const response = await fetch(url, options);
    const data = await response.json();
    console.log(data)
    for (const [name, metadata] of Object.entries(data)){
      console.log(name)
      if(plugin_name == name){
        console.log("FOUND: ", plugin_name, " - Downloading its .wasm module file...")
        get_wasm_file(metadata.contentAddress)
      }
    }
  } catch (error) {
    console.error(error);
  }
}

// --------------------------------

require('dotenv').config();
const { ApiClient } = require('@twurple/api');
const { ChatClient } = require('@twurple/chat');
const { StaticAuthProvider } = require('@twurple/auth');
const express = require('express');

// Set up express server
const app = express();
const PORT = 5309;

// Channel for Twitch updates
const chatUpdates = [];

// Twitch worker function
async function twitchWorker() {
  const twitchUser = process.env.TWITCH_USER.trim();
  const clientId = process.env.CLIENT_ID.trim();
  const clientSecret = process.env.CLIENT_SECRET.trim();

  // Set up authentication for Twurple
  const authProvider = new StaticAuthProvider(clientId, clientSecret);
  // Initialize chat client
  const chatClient = new ChatClient({ authProvider, channels: [twitchUser] });

  // Handle incoming chat messages
  chatClient.onMessage(async (channel, user, message, msg) => {
    console.log(`${user}: ", ${message}`)
    
    if (message.startsWith('!')) {
      const parts = message.slice(1).split(' ');

      if (parts.length < 2) {
        await chatClient.say(channel, `${user} - Send a message that follows the format ![0-15] [0.0-10.0]`);
        return;
      }

      const channelNum = parseInt(parts[0], 10);
      const value = parseFloat(parts[1]);

      if (isNaN(channelNum) || channelNum < 0 || channelNum > 15) {
        await chatClient.say(channel, `${user} - Please enter a channel 0-15`);
        return;
      }

      if (isNaN(value) || value < 0.0 || value > 10.0) {
        await chatClient.say(channel, `${user} - Please enter a value between 0.0 and 10.0`);
        return;
      }

      if (chatUpdates.length === 16) {
        await chatClient.say(channel, `${user} - The command buffer is full, please wait for it to empty`);
      } else {
        await chatClient.say(channel, `${user} - Your command will be applied shortly`);
        chatUpdates.push(`${parts[0]} ${parts[1]}`);
      }
    }
  });

  // Handle user joining a channel
  chatClient.onJoin((channel, user) => {
    chatClient.say(channel, `WELCOME: ${user}`);
  });

  // Connect to Twitch chat
  await chatClient.connect();
  console.log(`Joined ${twitchUser}...`);
}

// ----- TODO: Reinstate when ready to integrate with twitch -----

// // JSON server worker function
// app.get('/twitch-queue', (req, res) => {
//   const data = {};

//   while (chatUpdates.length > 0) {
//     const msg = chatUpdates.shift(); // Get the oldest message
//     console.log('Flushed message: ', msg);
//     const [channel, value] = msg.split(' ');
//     data[channel] = value;
//   }

//   res.json(data);
// });


// app.listen(PORT, () => {
//   console.log(`Server is running at 0.0.0.0:${PORT}`);
// });

// Start the workers
//twitchWorker();

// ---------------------------------------------------------------


get_wasm_by_plugin_name("zig_template")
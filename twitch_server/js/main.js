require('dotenv').config();
const { ApiClient } = require('@twurple/api');
const { ChatClient } = require('@twurple/chat');
const { StaticAuthProvider } = require('@twurple/auth');
const express = require('express');

const {WasmModuleQueue} = require('./moduleQueue.js');
const {get_wasm_by_plugin_name} = require('./xtpHandler.js');

get_wasm_by_plugin_name

// Set up express server
const app = express();
const PORT = 5310;
const XTP_TOKEN = process.env.XTP_TOKEN.trim();
const XTP_EXTENSION_ID = process.env.XTP_EXTENSION_ID.trim();

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

    // TODO: replace the level-setting with requests to enqueue wasm modules fetched from xtp
    // if (message.startsWith('!')) {
    //   const parts = message.slice(1).split(' ');

    //   if (parts.length < 2) {
    //     await chatClient.say(channel, `${user} - Send a message that follows the format ![0-15] [0.0-10.0]`);
    //     return;
    //   }

    //   const channelNum = parseInt(parts[0], 10);
    //   const value = parseFloat(parts[1]);

    //   if (isNaN(channelNum) || channelNum < 0 || channelNum > 15) {
    //     await chatClient.say(channel, `${user} - Please enter a channel 0-15`);
    //     return;
    //   }
    // }
    // TODO: ------------------------------------------------------------------------

  });

  // Handle user joining a channel
  chatClient.onJoin((channel, user) => {
    chatClient.say(channel, `WELCOME: ${user}`);
  });

  // Connect to Twitch chat
  chatClient.connect();
  console.log(`Joined ${twitchUser}...`);
}

// TODO: Reinstate when ready to integrate with twitch
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
// TODO: ----------------------------------------------

// TODO: Using this for testing the communication with the VCV Module, remove
ZIG_WASM_MODULE_BYTES = get_wasm_by_plugin_name("zig_template", XTP_EXTENSION_ID, XTP_TOKEN)
// TODO: --------------------------------------------------------------------

// This endpoint will be periodically called by the VCV Rack Module and expects a new module
// or some signal that nothing has changed in return 
app.get('/pop-module', (req, res) => {
  // implement a function for an express js function that returns the uint8 byte array
  // representing our wasm module. TO the requester it should look no different than a
  // normal GET file request
  console.log("triggered pop-module")
  return {
    buffer:ZIG_WASM_MODULE_BYTES
  }
});

// Start the VCV handler
app.listen(PORT, () => {
  console.log(`Server is running at 0.0.0.0:${PORT}`);
});

// Start the workers
//twitchWorker();

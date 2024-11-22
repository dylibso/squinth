// ----- XTP INTEGRATION TEST -----

import createClient from '@dylibso/xtp'
const xtpClient = await createClient({
  appId: 'app_01j66kjy8few8azm4bpdkwy6gz',
  token: String(process.env.XTP_TOKEN),
  // typescript plug-ins need WASI, this is a minor detail that isn't important at the moment
  useWasi: true
})

const GUEST_KEY = '452c8f24-a86e-4812-ac02-e9fe1a664624'
const EXT_NAME = 'vcv_plugin'

// change this implementation to include guest commands
async function getCommands() {
  return await xtpClient.listAvailablePlugins(
    EXT_NAME,
    GUEST_KEY,
  )
}

async function runSquinthCommand(commandName, message) {
  // our Extension Point is:             `vcv-plugin`
  // our export that we want to call is: `batch_compute_wf`
  const pluginFunc = xtpClient.extensionPoints.vcv_plugin.batch_compute_wf
  console.log(pluginFunc)
  const result = await pluginFunc(
    GUEST_KEY,
    message
  )
  return result
}

console.log(await getCommands())
console.log(await runSquinthCommand("COMMAND_NAME_NOT_USED", "foobar"))

// --------------------------------

// require('dotenv').config();
// const { ApiClient } = require('@twurple/api');
// const { ChatClient } = require('@twurple/chat');
// const { StaticAuthProvider } = require('@twurple/auth');
// const express = require('express');

// // Set up express server
// const app = express();
// const PORT = 5309;

// // Channel for Twitch updates
// const chatUpdates = [];

// // Twitch worker function
// async function twitchWorker() {
//   const twitchUser = process.env.TWITCH_USER.trim();
//   const clientId = process.env.CLIENT_ID.trim();
//   const clientSecret = process.env.CLIENT_SECRET.trim();

//   // Set up authentication for Twurple
//   const authProvider = new StaticAuthProvider(clientId, clientSecret);
//   // Initialize chat client
//   const chatClient = new ChatClient({ authProvider, channels: [twitchUser] });

//   // Handle incoming chat messages
//   chatClient.onMessage(async (channel, user, message, msg) => {
//     console.log(`${user}: ", ${message}`)
    
//     if (message.startsWith('!')) {
//       const parts = message.slice(1).split(' ');

//       if (parts.length < 2) {
//         await chatClient.say(channel, `${user} - Send a message that follows the format ![0-15] [0.0-10.0]`);
//         return;
//       }

//       const channelNum = parseInt(parts[0], 10);
//       const value = parseFloat(parts[1]);

//       if (isNaN(channelNum) || channelNum < 0 || channelNum > 15) {
//         await chatClient.say(channel, `${user} - Please enter a channel 0-15`);
//         return;
//       }

//       if (isNaN(value) || value < 0.0 || value > 10.0) {
//         await chatClient.say(channel, `${user} - Please enter a value between 0.0 and 10.0`);
//         return;
//       }

//       if (chatUpdates.length === 16) {
//         await chatClient.say(channel, `${user} - The command buffer is full, please wait for it to empty`);
//       } else {
//         await chatClient.say(channel, `${user} - Your command will be applied shortly`);
//         chatUpdates.push(`${parts[0]} ${parts[1]}`);
//       }
//     }
//   });

//   // Handle user joining a channel
//   chatClient.onJoin((channel, user) => {
//     chatClient.say(channel, `WELCOME: ${user}`);
//   });

//   // Connect to Twitch chat
//   await chatClient.connect();
//   console.log(`Joined ${twitchUser}...`);
// }

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

// // Start the workers
// twitchWorker();

Other issue I ran into was updating th extism library that I have installed which is different from the extism CLI or XTP that is installed, you need tom go into the extsim repo and make, make install to bring it up to date.

USE TWITCH-CLI app that twitch provides to get your oauth token, run "twitch configure" with CLIENT id and CLIENT secret, then run "twitch token" to get a new OAUTH
look at your dev console to get the CLIENT ID of your app that is used to generate the oauth

the  !!!! USER ID !!! is DIFFERENT THAN THE CLIENT ID, client id is just for the app and for generating oauth. This is a number assoc. with your account. Use this to find it (I'm sure there is another way)
https://www.streamweasels.com/tools/convert-twitch-username-%20to-user-id/

TWITCH TOKEN GENERATOR THAT I CONNECTED TO MY TWITCH ACCOUNT, can this be connected to Dylibso account safely?
https://twitchtokengenerator.com/
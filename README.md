# sQuinth by Dylibso!

A demonstration of collaborative audio programming via XTP, Extism, VCV Rack, and Twitch!

sQuinth connects Twitch, VCV Rack, and Dylibso's XTP to create a multi-language programmable oscillator that viewers can control through online chat messages.

![Module Panel Design](/vcv_module/res/sQuinth.png)

## Quick Start

Currently only tested on Linux, but if you'd like to create scripts on your platform that do what the .sh files in here do then it should work for you. If you are non-Linux visit the [install page for Extism](https://extism.org/docs/install/) to get started.

Download and install all prerequisites:
```
./get_prereqs.sh
```

Set your ```RACK_USER_DIR``` if it isn't already in your ```.bashrc```. This is where the directories that contain your patch files and your plugins live.
```
export RACK_USER_DIR=/home/<user>/.local/share/Rack2
```

Build the plugin:
```
./rebuild_all.sh
```

## Create A Plugin

1. Build the plugin template from the bundle found in the templates directory using the XTP CLI tool.
2. Implement ```wave()``` in the plugin
3. Build wasm binary for the plugin with XTP (optionally submit the plugin to the registry)
4. Load sQuinth in your VCV patch and load your Wasm module with the right-click menu.

These notes can be found in the file where users define their plugins (for each language)

```
A simple example of saw wave generator plugin

Phase represents the percentage progress that the wave has made through the wavelength
 10
   |\    |\    |\    @\    @
   | \   | \   | \   | \   |
 --|--@--|--@--|--\--|--\--|--
   |   \ |   \ |   \ |   \ |
   |    \|    \|    \|    \|
-10   ▲     ▲        ▲     ▲
      │     │        │     │
     0.5   0.5      0.0   0.0
              PHASE

Inputs 1, 2, and 3 are the current values (-10 to 10) of 3 "Low Frequency Oscillators",
sine waves that can be used to change certain factors over time. for example, amplitude.
Changing the amplitude would make the wave louder and quieter over time. Think of it like
automatically turning the volume knob up and down according to a slow sine wave.

"Modulation" is the word typically used to describe these sorts of procedural changes

the frequency is taken care of for you, but is is also provided here in hertz if you would
like the synth behvior to change (modulate) according the frequency it is generating

The fourth input increases from -10.0 to 10.0 over the course of every 16 bars, this can be
used to modulate the waveform over time.
```

## Communicate with Twitch and the XTP Plugin registry

Before you are able to run the go program that listens to Twitch and downloads plugins from your registry, you will need to create a ```.env``` file in the ```twitch_server/``` directory that contains:

- a Twitch oauth token with chat permissions
- the name of your Twitch channel
- your XTP token
- the extension ID for your plugins

```
XTP_TOKEN=""
XTP_EXTENSION_ID=""
TWITCH_OAUTH=""
CHANNEL=""
```

Then ```go run *.go```. The worker will listen to the chat messages and to requests for updates made by the VCV Rack module.

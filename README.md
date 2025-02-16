# sQuinth by Dylibso!

A demonstration of collaborative audio programming via XTP, Extism, VCV Rack, and Twitch!

![Module Panel Design](/vcv_module/res/sQuinth.png)

## Quick Start

Currently only tested on linux, but if you'd like to create scripts on your platform that do what the .sh files in here do then it should work for you. If you are non-linux visit the [install page for Extism](https://extism.org/docs/install/) to get started.

Download and install all prerequisites:
```
./get_prereqs.sh
```

Set your Rack User dir if it isn't already in your .bashrc. This is where the dirs that contain your patch files and your plugins live.
```
export RACK_USER_DIR=/home/<user>/.local/share/Rack2
```

Build the plugin:
```
./rebuild_all.sh
```

## Example Plugin Instructions

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

## TODO

- weird thing where when the js template runs for a minute or two it becomes glitched in a random looking way and cpu usage drops to zero, but calls to extism guest funcs is still reporting success codes...
- Implement more complex, varied wave types

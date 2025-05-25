# A simple example of saw wave generator plugin

# Phase represents the percentage progress that the wave has made through the wavelength
#  10
#    |\    |\    |\    @\    @
#    | \   | \   | \   | \   |
#  --|--@--|--@--|--\--|--\--|--
#    |   \ |   \ |   \ |   \ |
#    |    \|    \|    \|    \|
# -10   ▲     ▲        ▲     ▲
#       │     │        │     │
#      0.5   0.5      0.0   0.0
#               PHASE

# lfoOne and lfoTwo are the current values (-10 to 10) of two "Low Frequency Oscillators",
# waves that can be used to change certain factors over time. for example, amplitude.
# Changing the amplitude would make the wave louder and quieter over time. Think of it like
# automatically turning the volume knob up and down according to a slow sine wave.

# "Modulation" is the word typically used to describe these sorts of procedural changes

# the frequency is taken care of for you, but is is also provided here in hertz if you would
# like the synth behavior to change (modulate) according the frequency it is generating

from typing import Optional, List  # noqa: F401
from datetime import datetime  # noqa: F401
import extism  # noqa: F401 # pyright: ignore

import math

from pdk_types import WaveArgs  # noqa: F401


from typing import List, Optional  # noqa: F401


# User-defined function that computes a single sample of a waveform
def wave(input: WaveArgs) -> str:
    wave1_amp = math.sin(input.phase)

    # rescale -10..10 to -2..3 and clamp for use in equation 2**x
    w2_freq_mult = math.pow(2.0, math.floor((input.inputs[0] + 10.0) * 0.25) - 2.0)
    wave2_samples_in_wl = input.sampleRate / (w2_freq_mult * input.freq_hz)
    wave2_amp = math.sin(
        (math.remainder(input.sample, wave2_samples_in_wl) / wave2_samples_in_wl)
        + (
            (input.inputs[1] + 10.0) * 0.05
        )  # rescale -10..10 to 0..1 allowing phase offset
    )

    # rescale -10..10 to -2..3 and clamp for use in equation 2**x
    w3_freq_mult = math.pow(2.0, math.floor((input.inputs[2] + 10.0) * 0.25) - 2.0)
    wave3_samples_in_wl = input.sampleRate / (w3_freq_mult * input.freq_hz)
    wave3_amp = math.sin(
        (math.remainder(input.sample, wave3_samples_in_wl) / wave3_samples_in_wl)
        + (
            (input.inputs[3] + 10.0) * 0.05
        )  # rescale -10..10 to 0..1 allowing phase offset
    )

    amplitude = ((wave1_amp * wave2_amp * wave3_amp) - 0.5) * 20

    return amplitude

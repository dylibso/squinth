use crate::pdk::types;

use extism_pdk::*;

/**
 * A simple example of saw wave generator plugin
 */

// Phase represents the percentage progress that the wave has made through the wavelength
//  10
//    |\    |\    |\    @\    @
//    | \   | \   | \   | \   |
//  --|--@--|--@--|--\--|--\--|--
//    |   \ |   \ |   \ |   \ |
//    |    \|    \|    \|    \|
// -10   ▲     ▲        ▲     ▲
//       │     │        │     │
//      0.5   0.5      0.0   0.0
//               PHASE

// Inputs 1, 2, and 3 are the current values (-10 to 10) of 3 "Low Frequency Oscillators",
// waves that can be used to change certain factors over time. for example, amplitude.
// Changing the amplitude would make the wave louder and quieter over time. Think of it like
// automatically turning the volume knob up and down according to a slow sine wave.

// "Modulation" is the word typically used to describe these sorts of procedural changes

// the frequency is taken care of for you, but is is also provided here in hertz if you would
// like the synth behvior to change (modulate) according the frequency it is generating

// The fourth input increases from -10.0 to 10.0 over the course of every 16 bars, this can be
// used to modulate the waveform over time.

pub fn comp_amp(freq_hertz: f32, sample: i64, sample_rate: f32) -> f32 {
    let samples_in_wl: f32 = sample_rate / freq_hertz;
    let phase: f32 = (sample % samples_in_wl as i64) as f32 / samples_in_wl;
    (phase - 0.5) * 20.0
}

// User-defined function that computes a single sample of a waveform
pub(crate) fn wave(_input: types::WaveArgs) -> Result<f32, Error> {
    // ensures num_voices >= 1 at all times
    let num_voices: usize = usize::max(_input.inputs[0] as usize, 1);
    let mut amplitude: f32 = 0.0;
    for voice in 0..num_voices {
        let input_param = f32::abs(_input.inputs[1] + 10.0);
        let detune_exp: f32 = 1.0 + (voice as f32 * input_param * 0.005);
        let detune_amt: f32 = f32::powf(_input.freq_hz, detune_exp) - _input.freq_hz;
        let freq: f32 = if voice % 2 == 1 {
            _input.freq_hz - detune_amt
        } else {
            _input.freq_hz + detune_amt
        };

        amplitude += comp_amp(freq, _input.sample, _input.sample_rate);
    }
    amplitude /= num_voices as f32;

    Ok(amplitude)
}

use crate::pdk::types;

use extism_pdk::*;

/**
 * A simple example of saw wave generator plugin
 *
 *
 * Phase represents the percentage progress that the wave has made through the wavelength
 *  10
 *    |\    |\    |\    @\    @
 *    | \   | \   | \   | \   |
 *  --|--@--|--@--|--\--|--\--|--
 *    |   \ |   \ |   \ |   \ |
 *    |    \|    \|    \|    \|
 * -10   ▲     ▲        ▲     ▲
 *       │     │        │     │
 *      0.5   0.5      0.0   0.0
 *               PHASE
 *
 * Inputs 1, 2, and 3 are the current values (-10 to 10) of 3 "Low Frequency Oscillators",
 * waves that can be used to change certain factors over time. for example, amplitude.
 * Changing the amplitude would make the wave louder and quieter over time. Think of it like
 * automatically turning the volume knob up and down according to a slow sine wave.
 *
 * "Modulation" is the word typically used to describe these sorts of procedural changes
 *
 * the frequency is taken care of for you, but is is also provided here in hertz if you would
 * like the synth behvior to change (modulate) according the frequency it is generating
 *
 * The fourth input increases from -10.0 to 10.0 over the course of every 16 bars, this can be
 * used to modulate the waveform over time. */
pub fn comp_amp(freq_hertz: f32, sample: i64, sample_rate: f32, phase_offset: f32) -> f32 {
    let samples_in_wl: f32 = sample_rate / freq_hertz;
    let phase: f32 = (sample % samples_in_wl as i64) as f32 / samples_in_wl;
    ((phase + phase_offset).fract() - 0.5) * 20.0
}

// shift a pitch by proper exponential scale
pub fn pitch_shift(freq_hz: f32, amt: f32) -> f32 {
    freq_hz * f32::powf(2.0, amt)
}

// shift a freq up or down a certain number of perfect fifths
pub fn perfect_fifth(freq_hz: f32, steps: i32) -> f32 {
    freq_hz * f32::powf(2.0, 0.5 * (steps as f32))
}

// User-defined function that computes a single sample of a waveform
pub(crate) fn wave(_input: types::WaveArgs) -> Result<f32, Error> {
    // ensures num_voices >= 1 at all times
    let num_voices: usize = (_input.inputs[0] + 11.0) as usize;
    let mut amplitude: f32 = 0.0;
    for voice in 0..num_voices {
        let detune_input = (_input.inputs[1] + 10.0) * (1.0 / 20.0);
        let alt_mode: bool = detune_input > 0.98;

        let detune_freq: f32 = if voice == 0 {
            _input.freq_hz
        } else {
            // determine the number of steps up or down this voice will lie
            // if you have 7 voices (including original) you'll have the step amounts of -3, -2, -1, 1, 2, 3
            let step_delta: i32 = if voice % 2 == 0 {
                (voice as i32) / 2
            } else {
                ((voice as i32) + 1) / -2
            };

            if !alt_mode {
                // ok so the bigger steps actually equate to less offset, but thats just aesthetic
                pitch_shift(_input.freq_hz, detune_input / step_delta as f32)
            } else {
                // offset by perfect fifths, but don't go higher than 3 octaves
                //
                // wow, so rust's "%" operation is technically a "remainder" operation
                //  rather than a modulus like in other languages. The only time this
                //  matters is when you use negative values like we do half the time here.
                //  unlike modulus, remainder can return negative values
                //  -5 % 3 == -2 && -5 % -3 == -2 && 5 % -3 == 2 && 5 % 3 == 2

                perfect_fifth(_input.freq_hz, step_delta % 6)
            }
        };

        amplitude += comp_amp(
            detune_freq,
            _input.sample,
            _input.sample_rate,
            (voice as f32) * 0.05,
        );
    }

    amplitude /= num_voices as f32;

    Ok(amplitude)
}

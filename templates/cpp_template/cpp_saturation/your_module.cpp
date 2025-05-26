/**
 * A simple example of saw wave generator plugin
 */

// Phase represents the percentage progress that the wave has made through the
// wavelength
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

// lfoOne and lfoTwo are the current values (-10 to 10) of two "Low Frequency
// Oscillators", waves that can be used to change certain factors over time. for
// example, amplitude. Changing the amplitude would make the wave louder and
// quieter over time. Think of it like automatically turning the volume knob up
// and down according to a slow sine wave.

// "Modulation" is the word typically used to describe these sorts of procedural
// changes

// the frequency is taken care of for you, but is is also provided here in hertz
// if you would like the synth behvior to change (modulate) according the
// frequency it is generating

/**
 * User-defined function that computes a single sample of a waveform
 *
 * @param input
 * @return The computed sample
 */
std::expected<float, pdk::Error> impl::wave(pdk::WaveArgs &&input) {
  // Compute a saw wave oscillating between 10 and -10
  // float amplitude = (input.phase - 0.5) * 20.0;
  // return amplitude;
  // calculates output amplitude of distorted wave -10..10

  // const ws_input: F32 = (input.inputs[0] + 10.0) * (3.999 / 20.0);
  // const ws_index: I32 = Math.trunc(ws_input)
  // const ws_frac: F32 = ws_input - ws_index

  // const wave_vals = new Float32Array(4);
  // wave_vals[0] = -1 * Math.cos(2 * 3.1415 * input.phase); // sine
  // wave_vals[1] = (Math.round(input.phase) - 0.5) * 2.0; // square
  // wave_vals[2] = 2.0 * (input.phase - 0.5); // sawtooth
  // wave_vals[3] = -2.0 * Math.abs(2.0 * input.phase - 1) + 1; // triangle

  // // determine the value of the interpolated wave
  // const itpl_wv: F32 =
  //   ((1.0 - ws_frac) * wave_vals[ws_index]) +
  //   (ws_frac * wave_vals[(ws_index + 1) % 4]);

  // const rescaled_input: F32 = Math.max((input.inputs[1] + 10.0) / 2.0, 0.05);
  // const amplitude: F32 =
  //   (0.6366 * Math.atan(rescaled_input * itpl_wv)) *
  //   (1 / (0.6366 * Math.atan(rescaled_input))) * 10.0;

  // return amplitude;
}

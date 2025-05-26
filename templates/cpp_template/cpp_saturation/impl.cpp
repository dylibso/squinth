#include "pdk.gen.hpp"
#include <cmath>

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

  // Compute a saw wave oscillating between 10 and -10
  // float amplitude = (input.phase - 0.5) * 20.0;
  // return amplitude;
  // calculates output amplitude of distorted wave -10..10

  const float ws_input = (input.inputs[0] + 10.0) * (3.999 / 20.0);
  const int ws_index = int(std::floor(ws_input));
  const float ws_frac = ws_input - float(ws_index);

  std::vector<float> wave_vals(4, 0.0);
  wave_vals[0] = -1 * std::cos(2 * 3.1415 * input.phase);    // sine
  wave_vals[1] = (std::round(input.phase) - 0.5) * 2.0;      // square
  wave_vals[2] = 2.0 * (input.phase - 0.5);                  // sawtooth
  wave_vals[3] = -2.0 * std::abs(2.0 * input.phase - 1) + 1; // triangle

  // determine the value of the interpolated wave
  const float itpl_wv = ((1.0 - ws_frac) * wave_vals[ws_index]) +
                        (ws_frac * wave_vals[(ws_index + 1) % 4]);

  return itpl_wv * 10.0;
}

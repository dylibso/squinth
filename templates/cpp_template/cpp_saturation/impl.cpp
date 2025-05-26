#include "pdk.gen.hpp"

/**
 * User-defined function that computes a single sample of a waveform
 *
 * @param input
 * @return The computed sample
 */
std::expected<float, pdk::Error> impl::wave(pdk::WaveArgs &&input) {
  // Compute a saw wave oscillating between 10 and -10
  float amplitude = (input.phase - 0.5) * 20.0;
  return amplitude;
}

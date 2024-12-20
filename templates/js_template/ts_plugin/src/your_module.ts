import { WaveArgs } from "./pdk";

/**
 * User-defined function that computes a single sample of a waveform
 *
 * @param {WaveArgs} input -
 * @returns {number} The computed sample
 */
export function waveImpl(input: WaveArgs): number {
  const amplitude:F32 = (input.phase - 0.5) * 20.0; // calculates output amplitude of saw from 10 to -10 "modulated" by LFO One
  return amplitude;
}

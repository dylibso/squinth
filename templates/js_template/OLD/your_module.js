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

// lfoOne and lfoTwo are the current values (-10 to 10) of two "Low Frequency Oscillators", 
// waves that can be used to change certain factors over time. for example, amplitude. 
// Changing the amplitude would make the wave louder and quieter over time. Think of it like 
// automatically turning the volume knob up and down according to a slow sine wave.

// "Modulation" is the word typically used to describe these sorts of procedural changes 

export function wave(freq_hz, phase, input_one, input_two) {
  // Any frequency modulation should be done before the phase is calculated
  amplitude = (phase * -20.0) + 10.0 * lfoOne; // calculates output amplitude of saw from 10 to -10 "modulated" by LFO One
  return amplitude
}

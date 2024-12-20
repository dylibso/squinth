import "./pdk";
import { waveImpl } from "./your_module"


function batch_compute_wf() {
    let sample_time: f32 = input_buffer[0]
    let freq_hz: f32 = input_buffer[0]
    let mut curr_phase: f32 = input_buffer[0]

    let time_position: f32 = input_buffer[0]
    
    // TODO: FIX THIS
    let num_samples: i32 = i32::from_le_bytes(input[16..20].try_into().unwrap());

    let input_one: f32 = input_buffer[0]
    let input_two: f32 = input_buffer[0]

    // var input_buffer = new Float32Array(4).fill(0.0);
    var input_buffer = new Float32Array(Host.inputBytes());
    var floatbuf = new Float32Array(OUTBUF_SAMPLES).fill(0.0);
  
    for (let sample = 0; sample < floatbuf.length; sample++) {
        floatbuf[sample] = waveImpl(sample_time, freq_hz, lfo_one, lfo_two);
    }

  Host.outputBytes(floatbuf.buffer);
}

module.exports = { batch_compute_wf };
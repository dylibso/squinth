mod pdk;
use pdk::*;

mod your_module;
use your_module::*;

use extism_pdk::*;

const FLOAT32_BYTES: usize = 4;

#[plugin_fn]
pub fn batch_compute_wf(input: Vec<u8>) -> FnResult<Vec<u8>> {
    let sample_time: f32 = f32::from_le_bytes(input[0..4].try_into().unwrap());
    let freq_hz: f32 = f32::from_le_bytes(input[4..8].try_into().unwrap());
    let curr_phase: f32 = f32::from_le_bytes(input[8..12].try_into().unwrap());

    let num_samples: i32 = i32::from_le_bytes(input[12..16].try_into().unwrap());

    let input_one: f32 = f32::from_le_bytes(input[16..20].try_into().unwrap());
    let input_two: f32 = f32::from_le_bytes(input[20..24].try_into().unwrap());
    let input_three: f32 = f32::from_le_bytes(input[24..28].try_into().unwrap());
    let input_four: f32 = f32::from_le_bytes(input[28..32].try_into().unwrap());

    let is_left_channel: bool = i32::from_le_bytes(input[32..36].try_into().unwrap()) > 0;

    if num_samples < 1 {
        // Return unchanging nonzero output, 1024 samples is more than the host would usually ask for
        return Ok(vec![0u8; 1024 * FLOAT32_BYTES]);
    }

    let usize_num_samples: usize = num_samples.try_into().unwrap();

    let sample: i64 = i64::from_le_bytes(input[36..44].try_into().unwrap());
    let sample_rate: f32 = f32::from_le_bytes(input[44..48].try_into().unwrap());

    let mut outbuf: Vec<u8> = vec![0; usize_num_samples * FLOAT32_BYTES];

    for sample_no in 0..usize_num_samples {
        let wave_args: types::WaveArgs = types::WaveArgs {
            freq_hz,
            inputs: vec![input_one, input_two, input_three, input_four],
            phase: (curr_phase + (freq_hz * sample_time * sample_no as f32)).fract(),
            is_left_channel,
            sample: (sample + sample_no as i64),
            sample_rate,
        };

        // Computes the value of this individual sample and puts it where it belongs in the output buffer
        outbuf[sample_no * FLOAT32_BYTES..sample_no * FLOAT32_BYTES + 4]
            .copy_from_slice(&wave(wave_args).unwrap().to_le_bytes());
    }

    Ok(outbuf)
}

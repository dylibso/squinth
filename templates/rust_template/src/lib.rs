use extism_pdk::*;

const FLOAT32_BYTES:usize = 4;

// #[derive(serde::Deserialize)]
// #[derive(serde::Deserialize, FromBytes)]
// #[encoding(Json)]
// struct VcvInput {
//     sample_rate: f32,
//     sample_time: f32,
//     frame: i64,
//     pitch: f32,
//     phase: f32
// }

#[plugin_fn]
pub fn batch_compute_wf(input: Vec<u8>) -> FnResult<Vec<u8>> {
    let sample_time: f32 = f32::from_le_bytes(input[0..4].try_into().unwrap());
    let freq_hz: f32 = f32::from_le_bytes(input[4..8].try_into().unwrap());
    let mut curr_phase:f32 = f32::from_le_bytes(input[8..12].try_into().unwrap());

    let num_samples: i32 = i32::from_le_bytes(input[12..16].try_into().unwrap());

    let input_one: f32 = f32::from_le_bytes(input[16..20].try_into().unwrap());
    let input_two: f32 = f32::from_le_bytes(input[20..24].try_into().unwrap());
   
    if num_samples < 1 {
        // Return unchanging nonzero output, 1024 samples is more than the host would usually ask for
        return Ok(vec![1; 1024 * FLOAT32_BYTES])
    }
    else{}

    let usize_num_samples: usize = num_samples.try_into().unwrap();
    
    let mut outbuf: Vec<u8> = vec![0; usize_num_samples * FLOAT32_BYTES];

    for index in 0..usize_num_samples {
        curr_phase = (curr_phase + (freq_hz * sample_time)).fract();
        // Computes the value of this individual sample and puts it where it belongs in the output buffer
        outbuf[index * FLOAT32_BYTES..index * FLOAT32_BYTES + 4].copy_from_slice(
            &pitched_sine(freq_hz, curr_phase, input_one, input_two).unwrap().to_le_bytes(),
        );
    }

    Ok(outbuf)
}

pub fn pitched_sine(
    _freq_hz:f32, // No longer required since the phase is provided, but allows ppl to make synths that change with pitch
    phase:f32,
    input_one:f32,
    input_two:f32,
) -> FnResult<f32> {
    // Compute the sine output
    let sine_amplitude:f32 = f32::sin(2.0 * 3.14159 * phase) * input_one * input_two;
    Ok(sine_amplitude)
}
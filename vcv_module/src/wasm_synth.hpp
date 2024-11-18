#pragma once

#include <stdlib.h>
#include <string.hpp>
#include "../../extism/runtime/extism.h"

// TODO: pass the number of samples to compute as an arg
#pragma pack(1)
struct ExtismArgs
{
	float sample_time;
    float freq_hz;
	float input_one;
	float input_two;
};

ExtismPlugin* LoadExtismPlugin(std::string, bool);

float* ComputeAudioSamplesMonophonic(
    ExtismPlugin* plugin,
    float sample_time,
    float freq_hz,
    float inputs[6],
    size_t num_samples);

void ComputeAudioSamplesPolyphonic(
    ExtismPlugin* plugin,
    float sample_time,
    float freq_hz[4],
    float poly_output[4][256], // TODO: only 256 is supported due to coordination with wasm modules
    float inputs[6]);
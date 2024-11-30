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
    float curr_phase;
	float input_one;
	float input_two;
};

float ComputePhaseAfterNumSamples(
        int num_samples,
        float curr_phase,
        float sample_rate,
        float freq_hz);

ExtismPlugin* LoadExtismPlugin(std::string, bool);

float* ComputeAudioSamplesMonophonic(
    ExtismPlugin* plugin,
    float sample_time,
    float freq_hz,
    float inputs[6],
    size_t num_samples,
    float curr_phase
    );

void ComputeAudioSamplesPolyphonic(
    ExtismPlugin* plugin,
    float sample_time,
    float freq_hz[4],
    float poly_output[4][256], // TODO: only 256 is supported due to coordination with wasm modules
    float inputs[6]);
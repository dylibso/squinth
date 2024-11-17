#pragma once

#include <stdlib.h>
#include <string.hpp>
#include "../../extism/runtime/extism.h"

#pragma pack(1)
struct ExtismArgs
{
	float sample_time;
    float freq_hz;
	float lfo_one;
	float lfo_two;
};

ExtismPlugin* LoadFromPath(std::string);
float* ComputeAudioSamples(
    ExtismPlugin* plugin,
    float sample_time,
    float freq_hz,
    size_t num_samples,
    float lfo_one=0.0,
    float lfo_two=0.0);
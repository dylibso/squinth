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
  int32_t num_samples;
	float input_1;
	float input_2;
	float input_3;
	float input_4;
  int32_t is_left_channel;
  int64_t sample;
  float sample_rate;
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
    float inputs[4]=nullptr,
    int32_t num_samples=256, // TODO: only 256 is supported due to coordination with plugin modules
    float curr_phase=0.0, // If you do not set this, the wavelen will start from beginning at the end of each period
    bool is_left_channel=true,
    int64_t sample=0,
    float sample_rate=48.0
);

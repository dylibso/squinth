#include "wasm_synth.hpp"

// compute what the phase will be after a certain number of samples of the waveform have been
// computed at a particular sample rate
float ComputePhaseAfterNumSamples(
        int num_samples, float curr_phase, float sample_rate, float freq_hz){
    
    if (num_samples < 1) {
        return -1.0;
    }

    const float f_num_samples = float(num_samples);
    // use the sample frequency against the wave frequency to determine how many samples there are
    // in one period of the wave
    const float samples_in_one_period = sample_rate / freq_hz;
    const float periods_in_this_window = f_num_samples / samples_in_one_period;

    return fmod(periods_in_this_window + curr_phase, 1.0);
}

ExtismPlugin* LoadExtismPlugin(std::string path, bool is_url)
{
    std::string man_str;
    
    DEBUG("%ld", path.find("http://"));

    if(is_url){
        man_str = std::string("{\"wasm\": [{\"url\": \"" + path + "\"}]}");
    }
    else{
        man_str = std::string("{\"wasm\": [{\"path\":\"" + path + "\"}]}");
    }

    const char *manifest = man_str.c_str();
    char *errmsg = nullptr;

    ExtismPlugin *plugin = extism_plugin_new((const uint8_t *)manifest, strlen(manifest), NULL, 0, true, &errmsg); 
    
    // Throw error if failed to load a module from the file or URL
    if(errmsg || !plugin){
        DEBUG("Error Encountered While Attempting to Load Plugin:");
        DEBUG("%s", errmsg);
        throw errmsg;
    }
    
    return plugin;
}

float* ComputeAudioSamplesMonophonic(
    ExtismPlugin* plugin,
    float sample_time,
    float freq_hz,
    float inputs[4],
    int32_t num_samples, // TODO: only 256 is supported due to coordination with plugin modules
    float curr_phase, // If you do not set this, the wavelen will start from beginning at the end of each period
    bool is_left_channel,
    int64_t sample,
    float sample_rate
){
    if (plugin == NULL){
        DEBUG("ERROR: NULL PLUGIN\n");
        return nullptr;
    }
    else if (curr_phase > 1.0f || curr_phase < 0.0f)
    {
        DEBUG("Curr phase must be between 0.0 and 1.0");
        return nullptr;
    }

    ExtismArgs args = ExtismArgs{
        sample_time,
        freq_hz,
        curr_phase,
        num_samples,
        inputs ? inputs[0] : 1.0f,
        inputs ? inputs[1] : 1.0f,
        inputs ? inputs[2] : 1.0f,
        inputs ? inputs[3] : 1.0f,
        is_left_channel ? 1 : 0,
        sample,
        sample_rate
    };

    int rc = extism_plugin_call(plugin, "batch_compute_wf", (const uint8_t*)&args, sizeof(ExtismArgs));
    
    if (rc != EXTISM_SUCCESS) {
        DEBUG("EXTISM PLUGIN CALL FAILURE: %s", extism_plugin_error(plugin));
        return nullptr;
    }

    return (float*)extism_plugin_output_data(plugin);
}

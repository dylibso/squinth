#include "wasm_synth.hpp"

ExtismPlugin* LoadFromPath(std::string wasm_path)
{
    std::string man_str = std::string("{\"wasm\": [{\"path\":\"" + wasm_path + "\"}]}");
    const char *manifest = man_str.c_str();
    char *errmsg = nullptr;

    ExtismPlugin *plugin = extism_plugin_new((const uint8_t *)manifest, strlen(manifest), NULL, 0, true, &errmsg);
    
    if(!plugin) return nullptr;

    return plugin;
}

float* ComputeAudioSamples(
    ExtismPlugin* plugin,
    float sample_time,
    float freq_hz,
    size_t num_samples,
    float lfo_one,
    float lfo_two)
{
    ExtismArgs args = ExtismArgs{
        sample_time,
        freq_hz,
        lfo_one,
        lfo_two
    };

    if (plugin == NULL){
        DEBUG("ERROR: NULL PLUGIN\n");
    }
    
    int rc = extism_plugin_call(plugin, "batch_compute_wf", (const uint8_t*)&args, sizeof(ExtismArgs));
    
    if (rc != EXTISM_SUCCESS) {
        DEBUG("EXTISM PLUGIN CALL FAILURE: %s", extism_plugin_error(plugin));
        return nullptr;
    }

    return (float*)extism_plugin_output_data(plugin);
}

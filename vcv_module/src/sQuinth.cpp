#include "osdialog.h"
#include "plugin.hpp"
#include <patch.hpp>

char *strip_quotes(const char *str) {
  size_t len = strlen(str);

  // Check for empty string
  if (len < 2) {
    return NULL; // Return NULL if the string is too short
  }

  // Check if the first and last characters are quotes
  if (str[0] == '"' && str[len - 1] == '"') {
    // Allocate memory for new string (length - 2 for the quotes + 1 for null
    // terminator)
    char *result = (char *)malloc(len - 1);
    if (result) {
      strncpy(result, str + 1, len - 2); // Copy without quotes
      result[len - 2] = '\0';            // Null-terminate the new string
    }
    return result;
  }

  return NULL; // Return NULL if the string does not start and end with quotes
}

struct sQuinth : Module {

  enum ParamId { PARAMS_LEN };
  enum InputId {
    PITCH_INPUT,
    I1_INPUT,
    I2_INPUT,
    I3_INPUT,
    I4_INPUT,
    INPUTS_LEN
  };
  enum OutputId { OUT_L_OUTPUT, OUT_R_OUTPUT, OUTPUTS_LEN };
  enum LightId { LIGHTS_LEN };

  sQuinth() {
    config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
    configInput(PITCH_INPUT, "V/OCT");
    configInput(I1_INPUT, "Mod 1");
    configInput(I2_INPUT, "Mod 2");
    configInput(I3_INPUT, "Mod 3");
    configInput(I4_INPUT, "Mod 4");
    configOutput(OUT_L_OUTPUT, "Left");
    configOutput(OUT_R_OUTPUT, "Right");
  }

  std::string man_str = "";
  const char *manifest = man_str.c_str();

  char *errmsg = nullptr;
  ExtismPlugin *plugin = nullptr;

  int32_t cachesize = 256;

  const float *output_buf = nullptr;
  float *right_output_buf = new float[cachesize];

  TextDisplay *text_display = nullptr;

  float phase = 0.0f; // the current phase of the waveform

  void load_wasm(std::string path, bool is_url) {
    DEBUG("Loading wasm from supplied path %s", path.c_str());

    // Attempt to load a module from a file or URL.
    // Try/Catch avoids assignment if initialization fails
    try {
      ExtismPlugin *tmp_plugin = LoadExtismPlugin(path, is_url);
      if (plugin)
        free(plugin); // free old plugin
      plugin = tmp_plugin;
      tmp_plugin = nullptr;
    } catch (char *error_msg) {
      DEBUG("Failed to initialize extism module with message: %s", error_msg);
      return;
    }

    if (is_url) {
      this->text_display->text = "module";
      try {
        // HACKY way to get the name of the module just fetched
        json_t *reqJ = json_object();
        json_t *resJ = network::requestJson(network::METHOD_GET,
                                            "0.0.0.0:5310/name-queue", reqJ);
        const char *key;
        json_t *value;
        json_object_foreach(resJ, key, value) {
          /* block of code that uses key and value */
          const char *val = json_dumps(value, JSON_ENCODE_ANY);

          this->text_display->text = strip_quotes(val);
          DEBUG("--- key: %s - value: %s ---", key, val);
        }
        json_decref(reqJ);
        json_decref(resJ);

      } catch (const std::exception &e) {
        return;
      }
    } else {
      // use the file name as the module name
      std::string label_string =
          path.substr(path.rfind("/") + 1, path.rfind(".wasm"));
      this->text_display->text = label_string.c_str();
      DEBUG("Label String: %s", label_string.c_str());
    }

    DEBUG("Plugin Loaded Successfully");

    return;
  }

  void process(const ProcessArgs &args) override {

    // Periodically check the server for new modules
    // offset is meant to hackily make sure that the bd-cnct and squinth modules
    // do not check during the same frame, to alleviate spikes
    if ((args.frame + 444) % int64_t(args.sampleRate) * 3 == 0) {
      load_wasm("http://0.0.0.0:5310/module-queue", true);
    }

    // exit early if the plugin has not been set
    if (plugin == nullptr) {
      if (args.frame % 100000 == 0) {
        DEBUG("PLUGIN IS NULL");
      }
      outputs[OUT_L_OUTPUT].setVoltage(0.0f);
      outputs[OUT_R_OUTPUT].setVoltage(0.0f);
      return;
    }

    // Fill the Output buffer with new audio data
    if (args.frame % cachesize == 0) {

      const float freq_hz =
          261.6256 * std::pow(2.0, inputs[PITCH_INPUT].getVoltage());

      float wasm_inputs[6] = {
          inputs[I1_INPUT].isConnected() ? (float)inputs[I1_INPUT].getVoltage()
                                         : 0.0f,
          inputs[I2_INPUT].isConnected() ? (float)inputs[I2_INPUT].getVoltage()
                                         : 0.0f,
          inputs[I3_INPUT].isConnected() ? (float)inputs[I3_INPUT].getVoltage()
                                         : 0.0f,
          inputs[I4_INPUT].isConnected() ? (float)inputs[I4_INPUT].getVoltage()
                                         : 0.0f};

      // compute the right channel's audio samples
      output_buf = ComputeAudioSamplesMonophonic(
          plugin, args.sampleTime, freq_hz, wasm_inputs, cachesize, phase,
          false, args.frame, args.sampleRate);

      if (output_buf == nullptr) {
        DEBUG("ERROR: Output buffer is NULL after "
              "ComputeAudioSamplesMonophonic()");
        return;
      }

      memcpy(right_output_buf, output_buf, cachesize * sizeof(float));

      if (outputs[OUT_R_OUTPUT].isConnected()) {
        output_buf = ComputeAudioSamplesMonophonic(
            plugin, args.sampleTime, freq_hz, wasm_inputs, cachesize, phase,
            true, args.frame, args.sampleRate);
      }

      // update the current phase
      phase = ComputePhaseAfterNumSamples(cachesize, phase, args.sampleRate,
                                          freq_hz);
    }

    if (output_buf != nullptr) {
      outputs[OUT_L_OUTPUT].setVoltage(output_buf[args.frame % cachesize]);
      if (outputs[OUT_R_OUTPUT].isConnected()) {
        outputs[OUT_R_OUTPUT].setVoltage(
            right_output_buf[args.frame % cachesize]);
      }
    }

    return;
  }
};

std::string selectPathVCV() {
  std::string proj_path =
      APP->patch->path.substr(0, APP->patch->path.rfind("/") + 1);

  osdialog_filters *filters = osdialog_filters_parse("WASM:wasm");
  char *filename =
      osdialog_file(OSDIALOG_OPEN, proj_path.c_str(), NULL, filters);
  osdialog_filters_free(filters);
  std::string filename_string(filename);
  DEBUG("Returning selected path to file: %s", filename_string.c_str());
  return (filename_string);
}

// ----- Adapted from VoxGlitch WavBank Source code -----
// https://github.com/clone45/voxglitch/blob/master/src/WavBank/WavBankWidget.hpp
struct WasmPathItem : MenuItem {
  sQuinth *module;

  void onAction(const event::Action &e) override {
    pathSelect(module, selectPathVCV());
  }

  static void pathSelect(sQuinth *module, std::string path) {
    DEBUG("Path Select Function");
    if (path != "") {
      module->load_wasm(path, false);
    }
  }
};
// ------------------------------------------------------

struct WasmURLItem : MenuItem {
  sQuinth *module;

  void onAction(const event::Action &e) override {
    module->load_wasm("http://0.0.0.0:5310/module-queue", true);
  }
};

struct sQuinthWidget : ModuleWidget {
  BGPanel *pBackPanel;

  sQuinthWidget(sQuinth *module) {
    setModule(module);
    setPanel(createPanel(asset::plugin(pluginInstance, "res/sQuinth.svg")));

    pBackPanel = new BGPanel();
    pBackPanel->box.size = box.size;
    pBackPanel->imagePath = asset::plugin(pluginInstance, "res/sQuinth.png");
    pBackPanel->visible = false;
    addChild(pBackPanel);
    pBackPanel->visible = true;

    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(21.609, 32.823)), module,
                                             sQuinth::I1_INPUT));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(33.313, 18.636)), module,
                                             sQuinth::I2_INPUT));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(47.639, 25.073)), module,
                                             sQuinth::I3_INPUT));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(53.387, 12.821)), module,
                                             sQuinth::I4_INPUT));

    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(68.622, 06.884)), module,
                                             sQuinth::PITCH_INPUT));

    addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(126.485, 90.871)),
                                               module, sQuinth::OUT_L_OUTPUT));
    addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(145.156, 89.948)),
                                               module, sQuinth::OUT_R_OUTPUT));

    TextDisplay *text_display = createWidget<TextDisplay>(
        Vec(36.0 * MM_TO_NVG_UNITS, 67.0 * MM_TO_NVG_UNITS));
    text_display->box.size =
        Vec(58.0 * MM_TO_NVG_UNITS, 36.00 * MM_TO_NVG_UNITS);

    addChild(text_display);
    if (module) {
      module->text_display = text_display;
    }
  }

  void appendContextMenu(Menu *menu) override {
    WasmPathItem *filepath_set_module_entry = new WasmPathItem;
    filepath_set_module_entry->text = "Set Path to Wasm Module";
    filepath_set_module_entry->module = dynamic_cast<sQuinth *>(this->module);
    menu->addChild(filepath_set_module_entry);

    WasmURLItem *url_set_module_entry = new WasmURLItem;
    url_set_module_entry->text = "Load Wasm Module from URL";
    url_set_module_entry->module = dynamic_cast<sQuinth *>(this->module);
    menu->addChild(url_set_module_entry);
  }
};

Model *modelsQuinth = createModel<sQuinth, sQuinthWidget>("sQuinth");

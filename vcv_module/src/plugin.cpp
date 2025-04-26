#include "plugin.hpp"

Plugin *pluginInstance;
extern Model *modelsQuinth;

void init(Plugin *p) {
  pluginInstance = p;

  // Add modules here
  p->addModel(modelsQuinth);

  // Any other plugin initialization may go here.
  // As an alternative, consider lazy-loading assets and lookup tables when your
  // module is created to reduce startup times of Rack.
}

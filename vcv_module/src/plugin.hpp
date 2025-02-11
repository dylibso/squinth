#pragma once
#include <rack.hpp>
#include <stdio.h>
#include "wasm_synth.hpp"

using namespace rack;

// Declare the Plugin, defined in plugin.cpp
extern Plugin* pluginInstance;

// Declare each Model, defined in each module source file
// extern Model* modelMyModule;

struct TextDisplay : TransparentWidget {
  std::string text = "Use The Menu to set the Wasm Module";
  
  std::shared_ptr<Font> font;

  TextDisplay ( ) {
    font = APP->window->loadFont(asset::plugin(pluginInstance, "res/UbuntuMono-Regular.ttf"));
  }

  void changeText (std::string display_text) {
    this->text = display_text;
  }

  void draw (const DrawArgs &args) override {
    nvgFontSize(args.vg, 16);
    nvgFontFaceId(args.vg, font->handle);
    nvgTextLetterSpacing(args.vg, 1);

    nvgBeginPath(args.vg);
    nvgFillColor(args.vg, nvgRGBA(0xFF, 0xFF, 0xFF, 0xFF));

    nvgText(args.vg, 0, 0, this->text.c_str(), NULL);
  }
};

// LIFTED FROM https://github.com/netboy3/MSM-vcvrack-plugin/ MSMPanel in MSMComponentLibrary

struct BGPanel : TransparentWidget {
  NVGcolor backgroundColor;
  std::string imagePath;
	void draw(const DrawArgs &args) override {
      std::shared_ptr<Image> backgroundImage = APP->window->loadImage(imagePath);
	  nvgBeginPath(args.vg);
	  nvgRect(args.vg, 0.0, 0.0, box.size.x, box.size.y);

	  // Background color
	  if (backgroundColor.a > 0) {
	    nvgFillColor(args.vg, backgroundColor);
	    nvgFill(args.vg);
	  }

	  // Background image
	  if (backgroundImage) {
	    int width, height;
	    nvgImageSize(args.vg, backgroundImage->handle, &width, &height);
		float scale = 2.95;
	    NVGpaint paint = nvgImagePattern(args.vg, 0.0, 0.0, 187.960 * scale, 128.5 * scale, 0.0, backgroundImage->handle, 1.0);
	    nvgFillPaint(args.vg, paint);
	    nvgFill(args.vg);
	  }

	  // Border
	  NVGcolor borderColor = nvgRGBAf(0.5, 0.5, 0.5, 0.5);
	  nvgBeginPath(args.vg);
	  nvgRect(args.vg, 0.5, 0.5, box.size.x - 1.0, box.size.y - 1.0);
	  nvgStrokeColor(args.vg, borderColor);
	  nvgStrokeWidth(args.vg, 1.0);
	  nvgStroke(args.vg);

	  Widget::draw(args);
	}
};

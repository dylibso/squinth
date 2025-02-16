#pragma once
#include <rack.hpp>
#include <stdio.h>
#include "wasm_synth.hpp"

const float MM_TO_NVG_UNITS = 2.957;

using namespace rack;

// Declare the Plugin, defined in plugin.cpp
extern Plugin* pluginInstance;

// Declare each Model, defined in each module source file
// extern Model* modelMyModule;

struct TextDisplay : TransparentWidget {
  std::string text = "Use The Menu to set the Wasm Module";
  
  std::shared_ptr<Font> font;

  TextDisplay ( ) {
  }

  void changeText (std::string display_text) {
    this->text = display_text;
  }

  void draw (const DrawArgs &args) override {
    float text_left_margin = 6;

	nvgFontSize(args.vg, 16);
    nvgTextLetterSpacing(args.vg, 0);
    nvgFillColor(args.vg, nvgRGBA(255, 255, 255, 0xff));
    nvgTextAlign(args.vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
    float wrap_at = this->getSize()[0] - text_left_margin * MM_TO_NVG_UNITS; // width of the bounding box

    const char *end = NULL;
    NVGtextRow rows[4];
    unsigned int max_rows = 4;
    unsigned int number_of_lines = nvgTextBreakLines(args.vg, this->text.c_str(), NULL, wrap_at, rows, max_rows);

    if(number_of_lines > 2) end = rows[2].end;

    float bounds[4];
    nvgTextBoxBounds(args.vg, text_left_margin, 10, wrap_at, this->text.c_str(), end, bounds);

    float textHeight = bounds[3];
    nvgTextBox(args.vg, text_left_margin, (box.size.y / 2.0f) - (textHeight / 2.0f) + 8, wrap_at, this->text.c_str(), end);
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
	    NVGpaint paint = nvgImagePattern(args.vg, 0.0, 0.0, 187.960 * MM_TO_NVG_UNITS, 128.5 * MM_TO_NVG_UNITS, 0.0, backgroundImage->handle, 1.0);
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

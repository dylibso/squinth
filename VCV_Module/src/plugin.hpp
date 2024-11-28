#pragma once
#include <rack.hpp>
#include <network.hpp>

using namespace rack;

// Declare the Plugin, defined in plugin.cpp
extern Plugin* pluginInstance;

// Declare each Model, defined in each module source file
// extern Model* modelMyModule;
extern Model* modelBD_cnCt;

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
	    NVGpaint paint = nvgImagePattern(args.vg, 0.0, 0.0, 180.0 * 3.0, 380, 0.0, backgroundImage->handle, 1.0);
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

struct TransparentPort : SvgPort {
    TransparentPort() {
        // No SVG file is loaded, hence no visual element.
        setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/transparent.svg")));
        
        // Alternatively, if you want no SVG at all, comment the setSvg line.
    }

    // Optionally, override draw if needed to ensure no drawing occurs
    void draw(const DrawArgs& args) override {
        // Do nothing, so it's completely transparent visually
    }
};
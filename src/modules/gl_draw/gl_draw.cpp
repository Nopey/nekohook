
/*
 *
 *
 * Gl Draw using libglez
 *
 *
 */

#include "../../util/logging.hpp"
#include "../../util/colors.hpp"
#include "../../framework/gameticks.hpp" // Run on draw
#include "../../framework/drawing.hpp" // To
#include "../../framework/input.hpp" // For bounds
#include "embed.hpp"
#include "gl_draw.hpp"
#include <GL/glew.h>
#include <iostream>
namespace modules { namespace gl_draw {

// Convert frameworks colors into glez colors
static inline glez_rgba_t convert(CatVector4 in) {
  //Optimized away
  auto tmp = colors::FromRGBA8(in);
  return {tmp.x,tmp.y,tmp.z,tmp.a};
  //casts to unsigned char and back
  //return glez_rgba(in.x, in.y, in.z, in.a);
}

// Font system, We only use 12 fonts to save on memory
static glez_font_t glez_fonts[FONT_COUNT][12];

// Retrieves one of the fonts generated and saved into
// TODO fix
static const glez_font_t& GetGlezFont(int font, int size) {
	// Init fonts if we havent
	static bool font_init = false;
	if (!font_init) {
    // Per font
    int i=0;
    for(auto font:font_map){
      g_CatLogging.log("Found Font: %s",font.first.c_str());
      // Per size
			for (int ii = 0; ii < 12; ii++) {
        glez_fonts[i][ii] = glez_font_load_from_memory((void*)font.second->data(), (void*)font.second->size(), ii*2+20);
			}
      i++;
    }
    /*
		// Per font
		for (int i = 0; i < FONT_COUNT; i++) {
			// Per size
			for (int ii = 0; ii < 12; ii++) {
				//surface_fonts[i][ii] = g_ISurface->CreateFont();
				//g_ISurface->SetFontGlyphSet(surface_fonts[i][ii], draw::Fonts[i], ii + 20, 0, 0, 0, 0);
			}
		}*/
		font_init = true;
	}
	// Clamping as we dont have that many font sizes
	return glez_fonts[std::max(std::min(font,(int)font_map.size()),0)][std::max(std::min(32, size), 20)/2 - 10];
}

// Run to init glez
void Init() {
  auto error = glewInit();
  if (GLEW_OK != error){
    g_CatLogging.log("Error while initializing GLEW for gl_draw: %s",glewGetErrorString(error));
    return;
  }

  // Setup glez
  glez_init(input::bounds.first, input::bounds.second);

  // Glez draw before
  static auto last_bounds = input::bounds;
  drawmgr.REventBefore([](){
    // If our last bounds changed, we need to let glez know about the change
    if (last_bounds != input::bounds) {
      glez_resize(input::bounds.first, input::bounds.second);
      last_bounds = input::bounds;
    }

    // Let glez know we are starting draw
    glez_begin();
  });

  // Glez draw before, Let glez know we are ending draw
  drawmgr.REventAfter(glez_end);

  // Give the drawmgr our glez draw functions
  draw::Line = [](int x, int y, int w, int h, CatVector4 color) { glez_line(x, y, w, h, convert(color), 1); };
  draw::Rect = [](int x, int y, int w, int h, CatVector4 color) { glez_rect_outline(x, y, w, h, convert(color), 1); };
  draw::RectFilled = [](int x, int y, int w, int h, CatVector4 color) { glez_rect(x, y, w, h, convert(color)); };

  draw::Circle = [](int x, int y, float radius, int steps, CatVector4 color){ glez_circle(x, y, radius, convert(color), 1, steps); };

  draw::String = [](const char* text, int x, int y, int font, int size, const CatVector4 color) {
    float tmp1, tmp2;
    //glez_string_with_outline(x, y, text, font, convert(color), convert(colors::black), 2, 100, &tmp1, &tmp2);
    glez_string(x, y, text, GetGlezFont(font,size), convert(color), &tmp1, &tmp2);
  };
  draw::GetStringLength = [](const char* string, int font, int size) -> std::pair<int, int> {
    float tmp1, tmp2;
    glez_font_string_size(GetGlezFont(font,size), string, &tmp1, &tmp2);
    //return std::make_pair(tmp1 + 2, tmp2 + 2);
    return std::make_pair(tmp1, tmp2);
  };
}

}}

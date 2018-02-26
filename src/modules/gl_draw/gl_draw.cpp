
/*
 *
 *
 * Gl Draw using libglez
 *
 *
 */

#include "../../framework/gameticks.hpp" // Run on draw
#include "../../framework/drawing.hpp" // To
#include "../../framework/input.hpp" // For bounds
extern "C"{
#include "libglez/include/glez.h" // Include the lib_glez for easy open gl drawing ;)
}
#include "gl_draw.hpp"
#include <GL/glew.h>
#include <iostream>
namespace modules { namespace gl_draw {

// Convert frameworks colors into glez colors
static inline glez_rgba_t convert(CatVector4 in) {
  return {in.x,in.y,in.z,in.a};
}

// Font system, We only use 12 fonts to save on memory
//static glez_rgba_t glez_fonts[FONT_COUNT][12];

// Retrieves one of the fonts generated and saved into
// TODO fix
/*static const glez_rgba_t& GetGlezFont(int font, int size) {
	// Init fonts if we havent
	static bool font_init = false;
	if (!font_init) {
		// Per font
		for (int i = 0; i < FONT_COUNT; i++) {
			// Per size
			for (int ii = 0; ii < 12; ii++) {
				surface_fonts[i][ii] = g_ISurface->CreateFont();
				g_ISurface->SetFontGlyphSet(surface_fonts[i][ii], draw::Fonts[i], ii + 20, 0, 0, 0, 0);
			}
		}
		font_init = true;
	}
	// Clamping as we dont have that many font sizes
	return glez_fonts[font][std::max(std::min(32, size), 20) - 1];
}*/

// Run to init glez
void Init() {
  glewExperimental = GL_TRUE;
  auto error = glewInit();
  glClearColor(1.0, 0.0, 0.0, 0.5);
  if (GLEW_OK != error){
    std::cerr<<"Error Initializing GLEW for gl_draw: "<<glewGetErrorString(error)<<std::endl;
    return;
  }

  // Setup glez
  glez_init(input::bounds.first, input::bounds.second);

  // Glez draw before
  static auto last_bounds = input::bounds;
  drawmgr.REventBefore([](){
    //TODO: Detect Screen Resolution
    input::bounds.first=1920;
    input::bounds.second=1080;
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

  /*draw::String = [](const char* text, int x, int y, int font, int size, const CatVector4& color) {
    float tmp1, tmp2;
    glez_string_with_outline(x, y, text, font, convert(color), convert(colors::black), 2, 100, tmp1, tmp2);
  };
  draw::GetStringLength = [](const char* string, int font, int size) -> std::pair<int, int> {
    float tmp1, tmp2;
    glez_font_string_size(font, string, tmp1, tmp2);
    return std::make_pair(tmp1 + 2, tmp2 + 2);
  };*/
}

}}

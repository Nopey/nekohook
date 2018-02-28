
/*
 *
 *  Init for gl draw
 *
 */
#include "libglez/include/glez.h" // Include the lib_glez for easy open gl drawing ;)

namespace modules { namespace gl_draw {

void Init();
static const glez_font_t& GetGlezFont(int font, int size);
}}

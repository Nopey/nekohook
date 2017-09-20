

#ifndef DRAWMODMGR
#define DRAWMODMGR

#include <string.h> 
#include <functional>

#include "../util/colors.hpp"
#include "../util/mathlib.hpp"

enum EFont {
	NONE,
	OPENSANS,
	VERASANS,	// What was this again?
	UNISPACE,
	TF2BUILD	// Le meme
};

namespace drawmgr {
	

	
// Drawing Functions
void Line(int x, int y, int w, int h, rgba_t color);
void Rect(int x, int y, int w, int h, rgba_t color);
void RectFilled(int x, int y, int w, int h, rgba_t color);
void Circle(int x, int y, float radius, int steps, rgba_t color);
	
// Init Functions, these are fucked rn!
void InitLine (void *func(int, int, int, int, rgba_t));
void InitRect (void *func(int, int, int, int, rgba_t));
void InitRectFilled (void *func(int, int, int, int, rgba_t));
void InitCircle (void *func(int, int, float, int, rgba_t));
	
namespace strings {
void String(const char* text, int x, int y, EFont font, int size, rgba_t color);
void GetStringLength(const char* string, EFont font, int size, int& length, int& height);

void InitString (void *func(const char*, int, int, EFont, int, rgba_t));
void InitStringLength (void *func(const char*, EFont, int, int&, int&));
}
	
namespace other {
bool WorldToScreen(const CatVector& world, CatVector& screen);
void Reset();
void InitWorldToScreen (void *func(const CatVector&, CatVector&));
void InitReset(void *func());

}
	
}

#endif
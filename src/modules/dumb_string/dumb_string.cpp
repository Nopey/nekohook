#include "dumb_string.hpp"
#include "../../framework/gameticks.hpp" // Run on draw
#include "../../framework/drawing.hpp" // To
#include "../../framework/input.hpp" // For bounds
#include <cstring>

#define WM 0.82f
#define HM 1.65f
#define GAP 1
namespace modules { namespace dumb_string {
void Init() {
  draw::String = [](const char* text, int x, int y, int font, int size, CatVector4 color) {
    std::pair<int, int> dims = draw::GetStringLength(text,font,size);
    //draw::Rect(x,y,dims.first,dims.second,color);
    draw::Line(x,y,dims.first,dims.second,color);
    for(float chr = std::strlen(text);chr>0;chr--){
      draw::Rect(x+(chr-1)*size*WM+GAP, y+GAP, size*WM-GAP*2, size*HM-GAP*2, color);
    }
  };
  draw::GetStringLength = [](const char* string, int font, int size) -> std::pair<int, int> {
    int w, h;
    w = size * WM * std::strlen(string);
    h = size * HM;
    return std::make_pair(w, h);
  };
}
}}

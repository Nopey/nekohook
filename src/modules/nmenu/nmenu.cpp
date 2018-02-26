#include "nmenu.hpp"
#include "../../framework/drawing.hpp"
#include "../../framework/gameticks.hpp"

namespace modules { namespace nmenu {
void Init() {
    drawmgr.REventDuring([](){
        draw::Line(0,200,100,100,CatVector4(0,127,255,255));
        //text, x, y, font, size, color
        draw::String("Text Here", 100, 100, 0, 10, CatVector4(127,0,255,255));
    });
}
}}
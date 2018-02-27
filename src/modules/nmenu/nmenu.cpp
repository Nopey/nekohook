#include "nmenu.hpp"
#include "../../framework/drawing.hpp"
#include "../../framework/gameticks.hpp"

namespace modules { namespace nmenu {
void Init() {
    static int p=0;
    drawmgr.REventDuring([](){
        draw::Line(600+p/128%16,400+p/4%5,p/50%50*5-25,p/256%100*4,CatVector4(0,127,255,255));
        //text, x, y, font, size, color
        draw::String("Text", 100, 100, 0, 10, CatVector4(127,0,255,255));
        p++;
    });
}
}}

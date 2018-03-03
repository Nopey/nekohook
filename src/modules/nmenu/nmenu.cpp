#include "nmenu.hpp"
#include "../../framework/drawing.hpp"
#include "../../framework/gameticks.hpp"
#include "../../framework/input.hpp"
#include "../../util/colors.hpp"
#include "../../util/logging.hpp"

namespace modules { namespace nmenu {
void Init() {
    drawmgr.REventDuring([](){
        draw::Line(input::bounds.first/2,input::bounds.second/2,input::mouse.first-input::bounds.first/2,input::mouse.second-input::bounds.second/2, input::pressed_buttons[CATKEY_MOUSE_1]?colors::pink:CatVector4(0,127,255,255));
        //g_CatLogging.log("Color %f %f %f %f, %f %f %f %f",colors::pink.x,colors::pink.y,colors::pink.z,colors::pink.a,CatVector4(0,127,255,255).x,CatVector4(0,127,255,255).y,CatVector4(0,127,255,255).z,CatVector4(0,127,255,255).a);
        //text, x, y, font, size, color
    });
}
}}

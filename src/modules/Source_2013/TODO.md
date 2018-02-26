
all u need to do is hook CreateMove, 
and PaintTraverse,
 get enough interfaces to give nekohook info about entitys in src/framework/entitys.hpp,
  and run world tick in CreateMove, 
  and drawtick in PaintTraverse
if u look at /src/util/functions.hpp, CMFunction is for setting custom functions
magic!

Copied and Compiling:
- CreateMove
- Paint
- PaintTraverse
- hooks
- commandstack
- sdl
- interfaces
- pieces of sharedobj from cathook.
TODO:
- Run World Tick, DrawTick.
- Feed nekohook entity info.
- Integrate CMFunction into the cathook'd code to allow for modules
- Test basic functionality.
- Clean up: imports, CVars, TF2Classic & other games' things, 

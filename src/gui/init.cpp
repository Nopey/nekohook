
/*
 *
 *	This init find initializes all of the gui!
 *	Without this, gui elements may not function.
 *		-Onee
 *
 */

// Stuff to init with
#include "../framework/gameticks.hpp"		// So we can get drawmgr to draw our stuff
#include "../framework/input.hpp"			// For key_pressed & released callbacks

// Stuff to init
#include "gui.hpp"
#include "menu/menu.hpp"
#include "hudstrings/sidestrings.hpp"
#include "base/examples/test_window.hpp"

#include "init.hpp"

namespace gui {

void Init() {

	// Setup the draw manager to run gui
	drawmgr.REventDuring([](){g_pGui.Update();});
	input::key_pressed.add([](int key){g_pGui.OnKeyPress(key);});
	input::key_released.add([](int key){g_pGui.OnKeyRelease(key);});

	menu::Init();

	// Other gui elements
	sidestrings::Init();
	ShowTestWindow();
}

}

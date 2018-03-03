
/*
 * GUI.cpp
 *
 *  Created on: Jan 25, 2017
 *      Author: nullifiedcat
 */

#include "gui.hpp"

namespace gui {
CatEnum gui_menu({"GUI"}); // Menu locator for esp settings
CatVarKey mousekey(gui_menu, "gui_mousekey", CATKEY_CAPSLOCK, "GUI Mouse Toggle Button", "Freezes player look & unlocks mouse. (Keyboard shortcuts always work, ignorant of this setting)");
CatVarKey activatekey(gui_menu, "gui_activatekey", CATKEY_RIGHT, "GUI Activate Button", "Actuates. Presses buttons, opens submenus, etc.");
CatVarKey backkey(gui_menu, "gui_backkey", CATKEY_LEFT, "GUI Back-out Button", "Escapes. Closes submenus, deselects containers, etc.");
CatVarKey nextkey(gui_menu, "gui_nextkey", CATKEY_DOWN, "GUI Next Button", "Next element in container.");
CatVarKey prevkey(gui_menu, "gui_prevkey", CATKEY_UP, "GUI Prev Button", "Previous element in container.");
base::CBaseRoot g_pGui;

}

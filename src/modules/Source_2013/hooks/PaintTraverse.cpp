/*
 * PaintTraverse.cpp
 *
 *  Created on: Jan 8, 2017
 *      Author: nullifiedcat
 */

#include <vgui/IPanel.h>

#include <string.h>
#include "hookedmethods.hpp"
#include "hooks.hpp"
#include "../gameso.hpp"
#include "../../../util/logging.hpp"
#include "../../../util/catvars.hpp"
#include "../../../hacks/signature.hpp"
#include "../../../framework/gameticks.hpp"

CatVarBool clean_screenshots({"misc"}, "clean_screenshots", true,
                         "Clean screenshots",
                         "Don't draw visuals while taking a screenshot");
CatVarBool disable_visuals({"misc"}, "no_visuals", false, "Disable ALL drawing",
                       "Completely hides cathook");
CatVarBool no_zoom({"misc"}, "no_zoom", false, "Disable scope",
               "Disables black scope overlay");
CatVarBool pure_bypass({"misc"}, "pure_bypass", false, "Pure Bypass",
                   "Bypass sv_pure");
void *pure_orig  = nullptr;
void **pure_addr = nullptr;

CatEnum software_cursor_enum({ "KEEP", "ALWAYS", "NEVER", "MENU ON",
                               "MENU OFF" });
CatVarEnum
    software_cursor_mode({"misc"}, software_cursor_enum, "software_cursor_mode", 0,
                         "Software cursor",
                         "Try to change this and see what works best for you");

//TODO: Remove
#define PROF_SECTION(id)


void PaintTraverse_hook(void *_this, unsigned int vp, bool fr, bool ar)
{
    //TODO: World to screen transform
    static const PaintTraverse_t original = (PaintTraverse_t) hooks::panel.GetMethod(offsets::PlatformOffset(42));
    static bool textures_loaded      = false;
    static unsigned long panel_focus = 0;
    static unsigned long panel_scope = 0;
    static unsigned long panel_top   = 0;
    static bool cur, draw_flag = false;
    static bool call_default       = true;
    static ConVar *software_cursor = g_ICvar->FindVar("cl_software_cursor");
    static const char *name;
    static std::string name_s, name_stripped, reason_stripped;

    if (!textures_loaded)
    {
        textures_loaded = true;
        //TODO: Init Modules
    }
    if (pure_bypass)
    {
        if (!pure_addr)
        {
            pure_addr = *reinterpret_cast<void ***>(
                hacks::GetObjectSignature(gameso::engine(),
                    "55 89 E5 83 EC 18 A1 ? ? ? ? 89 04 24 E8 0D FF FF FF A1 ? "
                    "? ? ? 85 C0 74 08 89 04 24 E8 ? ? ? ? C9 C3") + 7 );
        }
        //If sv_pure is !false
        if (*pure_addr)
            //store sv_pure
            pure_orig = *pure_addr;
        //sv_pure=false
        *pure_addr    = (void *) 0;
    }
    else if (pure_orig)
    {
        //We've been told not to bypass sv_pure, but already have.
        //put it back.
        *pure_addr = pure_orig;
        pure_orig  = (void *) 0;
    }
    call_default = true;
    //TODO: reimplement master switch for cheats

    if (call_default)
        original(_this, vp, fr, ar);
    // To avoid threading problems.

    PROF_SECTION(PT_total);

    if (vp == panel_top)
        draw_flag = true;
    if (!panel_top)
    {
        name = g_IPanel->GetName(vp);
        if (strlen(name) > 4)
        {
            if (name[0] == 'M' && name[3] == 'S')
            {
                panel_top = vp;
            }
        }
    }
    if (!panel_focus)
    {
        name = g_IPanel->GetName(vp);
        if (strlen(name) > 5)
        {
            if (name[0] == 'F' && name[5] == 'O')
            {
                panel_focus = vp;
            }
        }
    }
    if (!panel_scope)
    {
        name = g_IPanel->GetName(vp);
        if (!strcmp(name, "HudScope"))
        {
            panel_scope = vp;
        }
    }

    if (vp != panel_focus)
        return;
    g_IPanel->SetTopmostPopup(panel_focus, true);
    if (!draw_flag)
        return;
    draw_flag = false;

    if (disable_visuals)
        return;

    if (clean_screenshots && g_IEngine->IsTakingScreenshot())
        return;
    //PROF_SECTION(PT_active);
    //g_CatLogging.log("PaintTraverse!");
}

/*
 * sdl.cpp
 *
 *  Created on: May 19, 2017
 *      Author: nullifiedcat
 */

#include "hookedmethods.hpp"
#include "sdl.hpp"
#include "../../../hacks/sharedobj.hpp"
#include "../../../util/logging.hpp"
#include "../../../framework/gameticks.hpp"
#include "../../../framework/input.hpp"
#include "../../gl_draw/gl_draw.hpp"
#include "../gameso.hpp"
#include "PaintTraverse.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <SDL2/SDL_events.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <map>

//TODO: Remove
#define PROF_SECTION(id)

SDL_GLContext sdl_gl_context{};
SDL_Window *sdl_current_window{ nullptr };
SDL_GL_SwapWindow_t *SDL_GL_SwapWindow_loc{ nullptr };
SDL_GL_SwapWindow_t SDL_GL_SwapWindow_o{ nullptr };

SDL_PollEvent_t *SDL_PollEvent_loc{ nullptr };
SDL_PollEvent_t SDL_PollEvent_o{ nullptr };

typedef uint32_t (*SDL_GetWindowFlags_t)(SDL_Window *window);

SDL_GetWindowFlags_t *SDL_GetWindowFlags_loc{ nullptr };
SDL_GetWindowFlags_t SDL_GetWindowFlags_o{ nullptr };

uint32_t SDL_GetWindowFlags_hook(SDL_Window *window)
{
    uint32_t flags = SDL_GetWindowFlags_o(window);
    flags &= ~(SDL_WINDOW_MINIMIZED | SDL_WINDOW_HIDDEN);
    flags |= SDL_WINDOW_SHOWN;
    return flags;
}

void SDL_GL_SwapWindow_hook(SDL_Window *window)
{
    //g_CatLogging.log("SDL_GL_SwapWindow..");
    static SDL_SysWMinfo wminfo;
    wminfo.version.major = 2;
    wminfo.version.minor = 0;
    typedef SDL_bool (*SDL_GetWindowWMInfo_t)(SDL_Window * window,
                                              SDL_SysWMinfo * info);
    static SDL_GetWindowWMInfo_t GetWindowWMInfo =
        *reinterpret_cast<SDL_GetWindowWMInfo_t *>(
            gameso::Pointer(gameso::libsdl(),0xFD4D8));//0003f9a9
    static bool init_wminfo{ false };
    if (!init_wminfo)
    {
        GetWindowWMInfo(window, &wminfo);
        init_wminfo = true;
    }
    if (!sdl_current_window)
        sdl_current_window = window;

    static bool init{ false };
    SDL_GLContext tf2_sdl = SDL_GL_GetCurrentContext();
    //TODO: cathook Master Toggle Switch
    if (/*cathook &&*/ !disable_visuals)
    {
        PROF_SECTION(SWAPWINDOW_cathook);
        if (not init)
        {
            //TODO: init graphics CMFunction
            sdl_gl_context = SDL_GL_CreateContext(window);
            SDL_GetWindowSize(window,&input::bounds.first,&input::bounds.second);
#if defined(CATHOOK_GFX_GL)
            g_CatLogging.log("Initializing gl_draw graphics module...");
            modules::gl_draw::Init();
#endif

            init = true;
        }
        SDL_GL_MakeCurrent(window, sdl_gl_context);
        drawmgr();
    }
    PROF_SECTION(SWAPWINDOW_tf2);
    SDL_GL_MakeCurrent(window, tf2_sdl);
    SDL_GL_SwapWindow_o(window);
    //g_CatLogging.log("Original worked..");
    // glXMakeContextCurrent(wminfo.info.x11.display,
    // wminfo.info.x11.window,
    //                      wminfo.info.x11.window, tf2);
    // glXSwapBuffers(wminfo.info.x11.display, wminfo.info.x11.window);
}


std::map<SDL_Keycode,int> sdl_keymap = {
	{SDLK_0, CATKEY_0}, {SDLK_1, CATKEY_1}, {SDLK_2, CATKEY_2},
	{SDLK_3, CATKEY_3}, {SDLK_4, CATKEY_4}, {SDLK_5, CATKEY_5},
	{SDLK_6, CATKEY_6}, {SDLK_7, CATKEY_7}, {SDLK_8, CATKEY_8},
	{SDLK_9, CATKEY_9}, {SDLK_a, CATKEY_A}, {SDLK_b, CATKEY_B},
	{SDLK_c, CATKEY_C}, {SDLK_d, CATKEY_D}, {SDLK_e, CATKEY_E},
	{SDLK_f, CATKEY_F}, {SDLK_g, CATKEY_G}, {SDLK_h, CATKEY_H},
	{SDLK_i, CATKEY_I}, {SDLK_j, CATKEY_J}, {SDLK_k, CATKEY_K},
	{SDLK_l, CATKEY_L}, {SDLK_m, CATKEY_M}, {SDLK_n, CATKEY_N},
	{SDLK_o, CATKEY_O}, {SDLK_p, CATKEY_P}, {SDLK_q, CATKEY_Q},
	{SDLK_r, CATKEY_R}, {SDLK_s, CATKEY_S}, {SDLK_t, CATKEY_T},
	{SDLK_u, CATKEY_U}, {SDLK_v, CATKEY_V}, {SDLK_w, CATKEY_W},
	{SDLK_x, CATKEY_X}, {SDLK_y, CATKEY_Y}, {SDLK_z, CATKEY_Z},

	{SDLK_ESCAPE, CATKEY_ESCAPE}, {SDLK_LEFTBRACKET, CATKEY_LBRACKET},
	{SDLK_RIGHTBRACKET, CATKEY_RBRACKET}, {SDLK_SEMICOLON, CATKEY_SEMICOLON},
	{SDLK_QUOTE, CATKEY_APOSTROPHE}, {SDLK_BACKQUOTE, CATKEY_BACKQUOTE},
	{SDLK_COMMA, CATKEY_COMMA}, {SDLK_PERIOD, CATKEY_PERIOD}, {SDLK_SLASH, CATKEY_SLASH},
	{SDLK_BACKSLASH, CATKEY_BACKSLASH}, {SDLK_MINUS, CATKEY_MINUS}, {SDLK_EQUALS, CATKEY_EQUAL},
	{SDLK_RETURN, CATKEY_ENTER}, {SDLK_SPACE, CATKEY_SPACE}, {SDLK_BACKSPACE, CATKEY_BACKSPACE},
	{SDLK_TAB, CATKEY_TAB}, {SDLK_CAPSLOCK, CATKEY_CAPSLOCK},

	{SDLK_INSERT, CATKEY_INSERT}, {SDLK_DELETE, CATKEY_DELETE},
	{SDLK_HOME, CATKEY_HOME}, {SDLK_END, CATKEY_END},
	{SDLK_PAGEUP, CATKEY_PAGEUP}, {SDLK_PAGEDOWN, CATKEY_PAGEDOWN},

	{SDLK_LSHIFT, CATKEY_LSHIFT}, {SDLK_RSHIFT, CATKEY_RSHIFT},
	{SDLK_LALT, CATKEY_LALT}, {SDLK_RALT, CATKEY_RALT},
	{SDLK_LCTRL, CATKEY_LCONTROL}, {SDLK_RCTRL, CATKEY_RCONTROL},

	{SDLK_KP_0, CATKEY_PAD_0}, {SDLK_KP_1, CATKEY_PAD_1}, {SDLK_KP_2, CATKEY_PAD_2},
	{SDLK_KP_3, CATKEY_PAD_3}, {SDLK_KP_4, CATKEY_PAD_4}, {SDLK_KP_5, CATKEY_PAD_5},
	{SDLK_KP_6, CATKEY_PAD_6}, {SDLK_KP_7, CATKEY_PAD_7}, {SDLK_KP_8, CATKEY_PAD_8},
	{SDLK_KP_9, CATKEY_PAD_9},

	{SDLK_KP_DIVIDE, CATKEY_PAD_DIVIDE}, {SDLK_KP_MULTIPLY, CATKEY_PAD_MULTIPLY},
	{SDLK_KP_MINUS, CATKEY_PAD_MINUS}, {SDLK_KP_PLUS, CATKEY_PAD_PLUS},
	{SDLK_KP_ENTER, CATKEY_PAD_ENTER}, {SDLK_KP_DECIMAL, CATKEY_PAD_DECIMAL},

	{SDLK_UP, CATKEY_UP}, {SDLK_LEFT, CATKEY_LEFT},
	{SDLK_DOWN, CATKEY_DOWN}, {SDLK_RIGHT, CATKEY_RIGHT},

	{SDLK_F1, CATKEY_F1}, {SDLK_F2, CATKEY_F2}, {SDLK_F3, CATKEY_F3},
	{SDLK_F4, CATKEY_F4}, {SDLK_F5, CATKEY_F5}, {SDLK_F6, CATKEY_F6},
	{SDLK_F7, CATKEY_F7}, {SDLK_F8, CATKEY_F8}, {SDLK_F9, CATKEY_F9},
	{SDLK_F10, CATKEY_F10}, {SDLK_F11, CATKEY_F11}, {SDLK_F12, CATKEY_F12},};

int SDL_PollEvent_hook(SDL_Event *event)
{
    int ret = SDL_PollEvent_o(event);
    if (ret)
    {
        //logging::Info("event %x %x", event->type, event->common.type);
        switch (event->type)
        {
            case SDL_KEYDOWN:{
                auto key = sdl_keymap.find(event->key.keysym.sym);
                if(key!=sdl_keymap.end()){
                    //g_CatLogging.log("BTN DWN %s",input::key_names[key->second]);
                    input::pressed_buttons[key->second]=true;
                    input::key_pressed(key->second);
                }
                break;
            }
            case SDL_KEYUP:{
                auto key = sdl_keymap.find(event->key.keysym.sym);
                if(key!=sdl_keymap.end()){
                    //g_CatLogging.log("BTN UP %s",input::key_names[key->second]);
                    input::pressed_buttons[key->second]=false;
                    input::key_released(key->second);
                }
                break;
            }
            case SDL_MOUSEMOTION:{
                input::mouse.first = event->motion.x;
                input::mouse.second = event->motion.y;
            }
            case SDL_MOUSEBUTTONDOWN:{
                auto key = CATKEY_MOUSE_1 + event->button.button - SDL_BUTTON_LEFT;
                input::pressed_buttons[key] = true;
                input::key_pressed(key);
                break;
            }
            case SDL_MOUSEBUTTONUP:{
                auto key = CATKEY_MOUSE_1 + event->button.button - SDL_BUTTON_LEFT;
                input::pressed_buttons[key] = false;
                input::key_released(key);
                break;
            }
            case SDL_WINDOWEVENT:{
                switch (event->window.event)
                {
                case SDL_WINDOWEVENT_RESIZED:
                    input::bounds.first = event->window.data1;
                    input::bounds.second = event->window.data2;
                    break;
                case SDL_WINDOWEVENT_HIDDEN:
                    g_CatLogging.log("Window Hidden");
                    break;
                case SDL_WINDOWEVENT_MINIMIZED:
                    g_CatLogging.log("Window Minimized");
                    break;
                }
                break;
            }
        }
    }
    return ret;
}

void DoSDLHooking()
{
    SDL_GL_SwapWindow_loc = reinterpret_cast<SDL_GL_SwapWindow_t *>(
        gameso::Pointer(gameso::libsdl(),0xFD648));

    SDL_GL_SwapWindow_o    = *SDL_GL_SwapWindow_loc;
    *SDL_GL_SwapWindow_loc = SDL_GL_SwapWindow_hook;
    

    //        SDL_GetWindowFlags_loc =
    //        reinterpret_cast<SDL_GetWindowFlags_t*>(sharedobj::libsdl().Pointer(0xFD588));

    //        SDL_GetWindowFlags_o = *SDL_GetWindowFlags_loc;
    //        *SDL_GetWindowFlags_loc = SDL_GetWindowFlags_hook;

    SDL_PollEvent_loc =reinterpret_cast<SDL_PollEvent_t *>(
        gameso::Pointer(gameso::libsdl(),0xFCF64));

    SDL_PollEvent_o = *SDL_PollEvent_loc;
    *SDL_PollEvent_loc = SDL_PollEvent_hook;
}

void DoSDLUnhooking()
{
    *SDL_GL_SwapWindow_loc = SDL_GL_SwapWindow_o;
    //*SDL_GetWindowFlags_loc = SDL_GetWindowFlags_o;
    *SDL_PollEvent_loc = SDL_PollEvent_o;
}

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
//#include <X11/Xlib.h>
//#include <GL/glx.h>

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

int SDL_PollEvent_hook(SDL_Event *event)
{
    int ret = SDL_PollEvent_o(event);
    if (ret)
    {
        //logging::Info("event %x %x", event->type, event->common.type);
        if (event->type == SDL_WINDOWEVENT)
        {
            switch (event->window.event)
            {
            case SDL_WINDOWEVENT_RESIZED:
                g_CatLogging.log(("Window Resized.. "+std::to_string(event->window.data1)+", "+std::to_string(event->window.data2)).c_str());
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

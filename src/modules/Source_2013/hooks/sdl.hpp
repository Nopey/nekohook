//For SDL_GLContext
#include <SDL.h>

/* SDL HOOKS */
//TODO: Move into sdl.hpp
union SDL_Event;
class SDL_Window;

extern SDL_Window *sdl_current_window;
extern SDL_GLContext sdl_gl_context;
typedef int (*SDL_PollEvent_t)(SDL_Event *event);
typedef void (*SDL_GL_SwapWindow_t)(SDL_Window *window);

int SDL_PollEvent_hook(SDL_Event *event);
void SDL_GL_SwapWindow_hook(SDL_Window *window);

void DoSDLHooking();
void DoSDLUnhooking();
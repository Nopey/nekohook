#include <unistd.h>

#include "init.hpp"
#include "../../hacks/hooking.hpp"
#include "../../util/logging.hpp"
#include "hooks/hooks.hpp"
#include "hooks/hookedmethods.hpp"
#include "hooks/PaintTraverse.hpp"
#include "hooks/CreateMove.hpp"
#include "hooks/sdl.hpp"
#include "interfaces.hpp"
#include <signal.h>

namespace modules{namespace source{
void Init(){
    CreateInterfaces();
    hooks::panel.Set(g_IPanel);
    hooks::panel.HookMethod((void *) PaintTraverse_hook,
                            offsets::PlatformOffset(42));
    hooks::panel.Apply();
    uintptr_t *clientMode = 0;
    // Bad way to get clientmode.
    // FIXME [MP]?
    while (!(
        clientMode = **(
            uintptr_t ***) ((uintptr_t)((*(void ***) g_IBaseClient)[10]) + 1)))
    {
        sleep(1);
    }
    hooks::clientmode.Set((void *) clientMode);
    hooks::clientmode.HookMethod((void *) CreateMove_hook, offsets::PlatformOffset(22));
    hooks::clientmode.Apply();
	g_CatLogging.log("Hooking SDL...");
	DoSDLHooking();
}
}}

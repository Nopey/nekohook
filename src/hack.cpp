
/*
 *	The cheats entry point is in entry.cpp, which creates a thread for cathook to use.
 *	From there, the init below here is run
 *
 */

#include <chrono> // For time so we can sleep
#include <thread> // Sleep in thread^
#include <ctime> // time()
#include <stdlib.h> // srand()

#include "util/logging.hpp" // To log progress

// Framework stuff
#include "framework/config.hpp"
#include "features/features.hpp"
#include "gui/init.hpp"

// TODO, ifdef this to shutup errors
// Game Modules
#if defined(CATHOOK_SOURCE_2013)
#include "modules/Source_2013/init.hpp"
#elif defined(CATHOOK_CSGO)
#include "modules/csgo/init.hpp"
#endif

#if defined(CATHOOK_GFX_GL)
#include "modules/gl_draw/gl_draw.hpp"
#endif

#if defined(CATHOOK_DUMB_STRING) 	// A dummy string graphic-alyzer
#include "modules/dumb_string/dumb_string.hpp"
#endif
#if defined(CATHOOK_UI_NMENU) 	// Basic UI
#include "modules/nmenu/nmenu.hpp"
#endif

#include "hack.h"

void hack::Initialize() {

	// We wait to allow the constructors to finish before we initialize
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	g_CatLogging.log("Initializing...");
	srand(time(0)); // Setting random seed?

	g_CatLogging.log("Initializing main cheat features...");
	features::Init();

	g_CatLogging.log("Initializing gui...");
	gui::Init(); // init the gui

	// Please put only one GAME module here. Any other type of module can screw with things.
	// Please also put compiler options to not compile the modules that are not needed.
	// Modules need to be initialized first to let the managers know what they can do.
	g_CatLogging.log("Loading Modules...");
#if defined(CATHOOK_SOURCE_2013) 	// Any source 2013 game
	modules::source::Init();
#elif defined(CATHOOK_CSGO) 	// CSGO
	modules::csgo::Init();
#endif

#if defined(CATHOOK_DUMB_STRING) 	// A dummy string graphic-alyzer
	g_CatLogging.log("Initializing dumb string...");
	modules::dumb_string::Init();
#endif
#if defined(CATHOOK_UI_NMENU) 	// Basic UI
	g_CatLogging.log("Initializing nmenu...");
	modules::nmenu::Init();
#endif

	// Load configs
	configs::Init();

	g_CatLogging.log("Initializing Done!");
}

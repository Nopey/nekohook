/*
 * CreateMove.cpp
 *
 *  Created on: Jan 8, 2017
 *      Author: nullifiedcat
 */

#include <link.h>
#include "../../../util/catvars.hpp"
#include "../interfaces.hpp"
#include "../../../hacks/hooking.hpp"
#include "../../../util/logging.hpp"
#include "../../../framework/gameticks.hpp"
#include "hooks.hpp"

//Instead of 5 includes, forward decl
class CUserCmd;
/*//All of these includes are required because I used CUserCmd
#include <basehandle.h>
#include <tier1/utlvector.h>
#include <mathlib.h>
#include <shareddefs.h> // requires mathlib & (engine/ivmodelrender.h or /tier1/utlvector.h)
#include <usercmd.h>// requires shareddefs and basehandle

//These ones are from cathook's CreateMove. Ignore 'em
#include <in_buttons.h>
#include <game/server/player.h>
#include <engine/ivmodelrender.h>
#include <icliententity.h>
#include <iprediction.h>
#include <inetchannel.h>
*/

bool CreateMove_hook(void *thisptr, float inputSample, CUserCmd *cmd)
{
    //g_CatLogging.log("CreateMove!");
    //if (g_IEngine->IsInGame()){
        wtickmgr();
    //}
}

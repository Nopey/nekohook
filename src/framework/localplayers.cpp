 
/*
 *
 *	This contains the entity for the local player and related options.
 *
 */

#include "localplayers.hpp"

void CLocalPlayer::Reset() {

	eye_position = CatVector(0, 0, 0); 
	attack = false;
}

// Var for our local player
CLocalPlayer g_LocalPlayer;

/*
 *
 *	The cheats main aimbot system.
 *
 *
 *
 */

#include "../framework/gameticks.hpp" // To run our stuff
#include "../framework/entitys.hpp"	// Contains entity and bone info
#include "../framework/input.hpp" // to get userinput for aimkeys
#include "../gui/hudstrings/sidestrings.hpp"
#include "esp.hpp"	// SetEspColor()

#include "aimbot.hpp"

namespace features { namespace aimbot {

static CatEnum aimbot_menu({"Aimbot"}); // Menu locator for esp settings
static CatVarBool enabled(aimbot_menu, "aimbot", true, "Enable Aimbot", "Main aimbot switch");
// Target Selection
static CatEnum priority_mode_enum({ "SMART", "FOV", "DISTANCE", "HEALTH" });
static CatVarEnum priority_mode(aimbot_menu, priority_mode_enum, "aimbot_prioritymode", 1, "Priority mode", "Priority mode.\nSMART: Basically Auto-Threat.\nFOV, DISTANCE, HEALTH are self-explainable.\nHEALTH picks the weakest enemy");
static CatVarFloat fov(aimbot_menu, "aimbot_fov", 0, "Aimbot FOV", "FOV range for aimbot to lock targets.", 180.0f);
static CatEnum teammates_enum({"ENEMY ONLY", "TEAMMATE ONLY", "BOTH"});
static CatVarEnum teammates(aimbot_menu, teammates_enum, "aimbot_teammates", 0, "Teammates", "Use to choose which team/s to target");
// Other
static CatVarKey aimkey(aimbot_menu, "aimbot_aimkey", CATKEY_MOUSE_1, "Aimkey", "If an aimkey is set, aimbot only works while key is depressed.");
static CatEnum hitbox_enum({ // Update this as needed
	"HEAD", "TOP SPINE", "UPPER SPINE", "MIDDLE SPINE", "BOTTOM SPINE", "PELVIS",
	"UPPER ARM L", "UPPER ARM R", "MIDDLE ARM L", "MIDDLE ARM R", "LOWER ARM L", "LOWER ARM R",
	"UPPER LEG L", "UPPER LEG R", "MIDDLE LEG L", "MIDDLE LEG R", "LOWER LEG L", "LOWER LEG R",
});
static CatVarEnum hitbox(aimbot_menu, hitbox_enum, "aimbot_hitbox", 0, "Hitbox", "Hitbox to use if mode is set to static");
static CatVarBool debug(aimbot_menu, "aimbot_debug", true, "debug", "gives debug info about aimbot");

// A function to find a place to aim for on the target
static CatVector RetriveAimpoint(const CatEntity& entity) {
	if (CE_BAD(entity)) return CatVector();

	// Check if we can use bones
	// Get our best bone
	CatVector tmp;
	if (bones::GetBone(entity, hitbox, tmp))
		return tmp;

	// Check for collision
	if (entity.collision != CatBox())
		// We can use the center collision for an aimpoint
		return entity.collision.center();

	// Without anything else, all we can use is the origin
	return entity.origin;
}

// Target Selection

// A second check to determine whether a target is good enough to be aimed at
static bool IsTargetGood(const CatEntity& entity) {
	if (CE_BAD(entity)) return false;

	// Local player check
	if (g_LocalPlayer.entity == &entity) return false;
	// Dead
	if (!entity.alive) return false;
	// Teammates
	switch(teammates) {
	case 0:	// Enemy only
		if (!entity.Enemy()) return false; break;
	case 1:	// Ally only
		if (entity.Enemy()) return false;
	}

	// Fov check
	if (fov > 0.0f && util::GetFov(RetriveAimpoint(entity)) > (float)fov) return false;

	// Hey look! Target passed all checks
	return true;
}
// Function to find a suitable target
static const CatEntity* RetrieveBestTarget() {

	// Book keepers for highest target
	const CatEntity* highest_ent = nullptr;
	float highest_score = -1024;

	// Loop through all entitys
	for (const auto& entity : g_CatEntitys) {
		// Ensure ent is okay to use
		if (CE_BAD(entity)) continue;

		// Check whether or not we can target the ent
		if (!IsTargetGood(entity)) continue;

		// Get score based on priority mode
		float score = 0;
		switch (priority_mode) {
		case 0: // SMART Priority
			score = 0; break; // TODO, fix
		case 1: // Fov Priority
			score = 360.0f - util::GetFov(RetriveAimpoint(entity)); break;
		case 2: // Distance priority
			score = 4096.0f - entity.Distance(); break;
		case 3: // Health Priority
			score = 1024.0f - entity.health;
		}

		// Compare the top score to our current ents score
		if (score > highest_score) {
			highest_score = score;
			highest_ent = &entity;
		}
	}

	return highest_ent;
}
// A check to determine whether the local player should aimbot
static bool ShouldAim() {

	// It would be prefered to have a local ent before we shoot
	if (!g_LocalPlayer.entity) return false;
	// Good check
	if (!g_LocalPlayer.entity->exists || g_LocalPlayer.entity->dormant) return false;
	// Alive check
	if (!g_LocalPlayer.entity->alive) return false;
	// aimkey
	if (aimkey && !input::pressed_buttons[aimkey]) return false;

	return true;
}

// Input a vector to aim at it
void AimAt(const CatVector& point) {
	g_LocalPlayer.SetCameraAngle(util::VectorAngles(g_LocalPlayer.GetCamera(), point));
}
// Input an entity to aim at it
void AimAt(const CatEntity& entity) {
	if (CE_BAD(entity)) return;
	AimAt(RetriveAimpoint(entity));
}

// The main "loop" of the aimbot.
static void WorldTick() {
	if (!enabled) return; // Main enabled check

	// Get our best target
	const CatEntity* target = RetrieveBestTarget();
	if (!target) return; // Check whether we found a target

	// Set targets color
	esp::SetEspColor(*target, colors::pink);	// Colors are cool

	// Check if our local player is ready to aimbot
	if (!ShouldAim()) return;

	AimAt(*target);
}

static void DrawTick() {
	if (debug) {
		char buf[128];
		sprintf(buf, "Camera Angle: %f, %f", g_LocalPlayer.GetCameraAngle().x, g_LocalPlayer.GetCameraAngle().y);
		gui::sidestrings::SideStrings.AddString(buf);
		sprintf(buf, "Camera Position: %f, %f, %f", g_LocalPlayer.GetCamera().x, g_LocalPlayer.GetCamera().y, g_LocalPlayer.GetCamera().z);
		gui::sidestrings::SideStrings.AddString(buf);
	}
}

void Init() {
	wtickmgr_on(WorldTick);
	drawmgr_on(DrawTick);
}

}}

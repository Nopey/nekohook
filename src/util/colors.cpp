
/*
 *
 *	This is the colors file, its used for everything related to color!
 *	Right now, go find the nearest walmart and buy some spray paint.
 * 	Spray the paint into your mouth.
 *	You are now officially gay or suicidal, your choice!
 *		-Onee
 *
 *
 */

#include <math.h>	// fabs() sin()
#include <chrono>	// For time keeping
#include <ratio> // above

#include "../features/aimbot.hpp"

#include "colors.hpp"

namespace colors {

// Use this to get a color from an entity!
CMFunction<CatVector4(CatEntity*)> EntityColor {
	[](CatEntity* entity) {

	// Default color
	CatVector4 ent_color = white;

	// Aimbot Color
	if (entity == features::aimbot::last_target)
		return colors::pink;

	// Different strokes for different folks
	auto ent_type = GetType(entity);
	if (ent_type == ETYPE_PLAYER || ent_type == ETYPE_OTHERHOSTILE || ent_type == ETYPE_PROJECTILE) {
		switch(GetTeam(entity)) {
		case ETEAM_RED:
			ent_color = red; break;
		case ETEAM_BLUE:
			ent_color = blue; break;
		case ETEAM_GREEN:
			ent_color = green; break;
		case ETEAM_YELLOW:
			ent_color = yellow; break;
		case ETEAM_ALLY:	// Blue good
			ent_color = blue; break;
		case ETEAM_ENEMY:	// Red bad
			ent_color = red;
		}
	}
	// Pickups
	switch(ent_type) {
	case ETYPE_PICKUP_HEALTH:
		ent_color = green; break;
	case ETYPE_PICKUP_SHEILD:
		ent_color = yellow;
	}
	return ent_color;
}};

// Returns a color based on entity health
CatVector4 Health(CatEntity* entity) {
	if (GetHealth(entity) > GetMaxHealth(entity))	// If health is too much, they must be over their normal health so we make them blue
		return CatVector4(64, 128, 255, 255);

	// Percentage of health our of max
	float hf = (float)GetHealth(entity) / (float)GetMaxHealth(entity);
	// Sick logic
	return CatVector4((hf <= 0.5 ? 1.0 : 1.0 - 2 * (hf - 0.5)) * 255, (hf <= 0.5 ? (2 * hf) : 1) * 255, 0, 255);
}

// Returns a rainbow color based on time
CatVector4 RainbowCurrent() {
	static auto start_time = std::chrono::steady_clock::now();
	std::chrono::duration<float, std::deca> curtime = std::chrono::steady_clock::now() - start_time;
	return colors::FromHSL(fabs(sin(curtime.count())) * 360.0f, 0.85f, 0.9f);
}

}

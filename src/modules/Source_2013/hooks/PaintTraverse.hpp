/*
 * PaintTraverse.h
 *
 *  Created on: Jan 8, 2017
 *      Author: nullifiedcat
 */

#pragma once
#include "../../../util/catvars.hpp"
extern CatVarBool no_zoom;
extern CatVarBool clean_screenshots;
extern CatVarBool disable_visuals;
void PaintTraverse_hook(void *, unsigned int, bool, bool);
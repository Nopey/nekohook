/*
 * CBaseWindow.h
 *
 *  Created on: Jan 25, 2017
 *      Author: nullifiedcat
 */

#pragma once

#include "CBaseContainer.hpp"

namespace gui { namespace base {

class CBaseWindow : public CBaseContainer {
public:
	// Constructors & Deconstructors
	CBaseWindow(std::string name = "unnamed", IWidget* parent = nullptr) : CBaseContainer(name, parent) {}
	virtual ~CBaseWindow() {};

	// General functions
	virtual void Draw() override;

	// User Input
	virtual void OnFocusGain() override;
	virtual void OnFocusLose() override;

	// Child related util
	virtual void MoveChildren() override;
};

}}
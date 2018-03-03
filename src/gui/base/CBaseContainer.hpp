
/*
 *
 *	An IWidget varient base designed to house child widgets
 *
 */

#pragma once

#include <vector>

#include "CBaseWidget.hpp"

namespace gui { namespace base {

class CBaseContainer : public CBaseWidget {
public:
	CBaseContainer(const char* _name);
	~CBaseContainer();

	virtual void Update();
	virtual void Draw();
	virtual void DrawBounds();

	// User input functions
	virtual void OnMouseLeave();
	virtual void OnMousePress();
	virtual void OnMouseRelease();
	virtual bool TryFocusGain();
	virtual void OnFocusLose();
	virtual void OnKeyPress(int key);
	virtual void OnKeyRelease(int key);
	virtual bool ConsumesKey(int key);

	// Visibility
	virtual void Hide();

	// Tooltips
	virtual const std::string& GetTooltip();

	// Children
	std::vector<IWidget*> children;
	void AddChild(IWidget* child);

	// Get Child/info
	IWidget* ChildByIndex(int idx);
	IWidget* ChildByName(const char* name);
	IWidget* ChildByPoint(int x, int y);

	// Child related util
	virtual void SortByZIndex();
	void UpdateHovers();
	virtual void MoveChildren();

	// Child info related to the container
	void HoverOn(IWidget* child);
	bool TryFocusOn(IWidget* child);
	void PressOn(IWidget* child);
	IWidget* hovered_child = nullptr;
	IWidget* focused_child = nullptr;
	IWidget* pressed_child = nullptr;
	// Not a guarantee
	bool can_focus_on_nothing = false;
};

}}

/*
 * CBaseContainer.cpp
 *
 *  Created on: Jan 31, 2017
 *      Author: nullifiedcat
 */

#include <cstring> // strcmp()
#include <algorithm> // std::sort

#include "../../framework/drawing.hpp" // Draw stuff
#include "../../util/colors.hpp" // Draw stuff
#include "../../framework/input.hpp"
#include "../gui.hpp"

#include "CBaseContainer.hpp"

namespace gui { namespace base {

using namespace gui;

// Constructor & destructor
CBaseContainer::CBaseContainer(const char* _name) : CBaseWidget(_name) {}
CBaseContainer::~CBaseContainer() { for (auto child : children) delete child; }

// General functions
void CBaseContainer::Update() {
	SortByZIndex();
	MoveChildren();
	UpdateHovers();
	for (auto child : children) {
		child->Update();
	}
	CBaseWidget::Update();
}
void CBaseContainer::Draw() {
	for (auto child : children) {
		if (child->IsVisible()) {
			child->Draw();
		}
	}
}
void CBaseContainer::DrawBounds() {
	auto abs_pos = AbsolutePosition();
	draw::Rect(abs_pos.first, abs_pos.second, size.first, size.second, focus ? (focused_child?colors::pink:colors::blue) : colors::gray);
	for (auto child : children) {
		if (child->IsVisible()) {
			child->DrawBounds();
		}
	}
	CBaseWidget::DrawBounds();
}

// User input functions
void CBaseContainer::OnMouseLeave() {
	HoverOn(0);
	CBaseWidget::OnMouseLeave();
}
void CBaseContainer::OnMousePress() {
	PressOn(ChildByPoint(input::mouse.first, input::mouse.second));
}
void CBaseContainer::OnMouseRelease() {
	if (pressed_child) pressed_child->OnMouseRelease();
}
bool CBaseContainer::TryFocusGain() {
	if(can_focus_on_nothing) return true;
	//TODO: Move this into it's own function.
	for(auto child : children){
		if(child->IsVisible()&&TryFocusOn(child)){
			return true;
		}
	}
	return false;
}
void CBaseContainer::OnFocusLose() {
	TryFocusOn(0);
	CBaseWidget::OnFocusLose();
}
void CBaseContainer::OnKeyPress(int key) {
	if(focused_child&&focused_child->ConsumesKey(key)){
		focused_child->OnKeyPress(key);
	}else{
		//Flip to next child
		if(key==nextkey.value){
			//TODO: Replace focused_child with the index of the selected child
			// so we don't have to spend time finding it here
			auto foc = std::find(children.begin(),children.end(),focused_child);
			//Increments, then compares.
			while(++foc!=children.end()){
				if((*foc)->IsVisible()&&TryFocusOn(*foc)){
					//We found a new child
					break;
				}
			}
		}
		//Flip to previous child
		else if (key==prevkey.value){
			auto foc = std::find(children.begin(),children.end(),focused_child);
			//Compares, then decrements.
			while(foc--!=children.begin()){
				if((*foc)->IsVisible()&&TryFocusOn(*foc)){
					//We found a new child
					break;
				}
			}
		}
		//Escape
		else if (key==backkey.value){
			if(can_focus_on_nothing){
				TryFocusOn(0);
			}
		}
		else if (key==activatekey.value){
			if(!focused_child){
				for(auto child:children){
					if(child->IsVisible()){
						TryFocusOn(child);
					}
				}
			}
		}
		
	}
}
void CBaseContainer::OnKeyRelease(int key) {
	//Tempted to just fully hand these key release events on, but that'll just slow things down.
	if (focused_child) focused_child->OnKeyRelease(key);
}
bool CBaseContainer::ConsumesKey(int key) {
	//If our focused child consumes, we can't
	return (focused_child && focused_child->ConsumesKey(key))
	//If we can go to prev/next child, do so.
	|| (key==nextkey && focused_child!=children.back())
	|| (key==prevkey && focused_child!=children.front())
	//If we can focus on nothing, and are currently focusing on something
	//(otherwise our parent container gets the backkey and will defocus our entire container)
	|| (key==backkey && can_focus_on_nothing && focused_child)
	//Or if we can `activate`
	|| (key==activatekey && focused_child==nullptr);
}

// Visiblity
void CBaseContainer::Hide() {
	CBaseWidget::Hide();
	if (hovered_child) hovered_child->OnMouseLeave();
	if (focused_child) focused_child->OnFocusLose();
	if (pressed_child) pressed_child->OnMouseRelease();
}

// Tooltips
const std::string& CBaseContainer::GetTooltip() {
	if (hovered_child) return hovered_child->GetTooltip();
	return tooltip;
}

// Children
void CBaseContainer::AddChild(IWidget* child) {
	children.push_back(child);
	child->parent = this;
}

// Children util
IWidget* CBaseContainer::ChildByIndex(int idx) {
	if (idx < 0 || idx >= children.size()) return nullptr;
	return children.at(idx);
}
IWidget* CBaseContainer::ChildByName(const char* name) {
	for (auto child : children) {
		if (child->name == name) {
			return child;
		}
	}
	return nullptr;
}
IWidget* CBaseContainer::ChildByPoint(int x, int y) { // Input a point in space to return a child within it
	for (int i = children.size() - 1; i >= 0; i--) {
		auto child = ChildByIndex(i);
		if (!child->visible) continue; // We dont care about always visible, we just want visible
		auto abs = child->AbsolutePosition();
		if (x >= abs.first && x <= abs.first + child->size.first &&
			y >= abs.second && y <= abs.second + child->size.second) {
			return child;
		}
	}
	return nullptr;
}

// Child related update functions
void CBaseContainer::SortByZIndex() {
	// Sort everything
	std::sort(children.begin(), children.end(), [](IWidget* a, IWidget* b) -> bool {
		return a->zindex < b->zindex;
	});
	// Make everything have a linear number... For delicious stacking
	for (int i = 0; i < children.size(); i++)
		children[i]->zindex = i;
}
void CBaseContainer::UpdateHovers() {
	auto hovered = ChildByPoint(input::mouse.first, input::mouse.second);
	if (hovered != hovered_child)
		HoverOn(hovered);
}
void CBaseContainer::MoveChildren() {
	// Used space
	std::pair<int, int> space = std::make_pair(-1, -1);
	// Get our absolutes down
	// Find a Bounding box around all of the absolutes
	for (auto c : children) {
		if (!c->IsVisible()) continue;
		// Check if not absolute
		if (c->position_mode != ABSOLUTE)
			continue;
		// Add the amount of space it takes to our used amount
		std::pair<int, int> space_taken;
		space_taken.first = c->offset.first + c->size.first;
		space_taken.second = c->offset.second + c->size.second;

		// If some our used space is less than the space taken by the widget, add it to used space.
		if (space.first < space_taken.first)
			space.first = space_taken.first;
		if (space.second < space_taken.second)
			space.second = space_taken.second;
	}

	// Get our size for the container and set it
	std::pair<int, int> tmp_max = (max_size == std::make_pair(-1, -1)) ? space : max_size;
	size = tmp_max;

	// Organize our inlines
	std::pair<int, int> cur_pos = std::make_pair(2, 2);
	int lane_height = 0;
	for (auto c : children) {
		if (!c->IsVisible()) continue;

		// Check if inline
		if (c->position_mode != INLINE)
			continue;

		// Get whether widget width would overlap max size, make widget go in new lane if true
		if (cur_pos.first + c->size.first + 2 > tmp_max.first) {

			// Put the widget on a new line
			cur_pos = std::make_pair(2, cur_pos.second + lane_height + 2);
			lane_height = 0;
		}

		// If our widget height is more than the lane size, add to it
		if (c->size.second > lane_height)
			lane_height = c->size.second;

		// Set the inline widgets position
		c->offset = cur_pos;

		// Add to the length used
		cur_pos.first += c->size.first + 2;
	}
}

// Child info related to the container
void CBaseContainer::HoverOn(IWidget* child) {
	if (hovered_child) hovered_child->OnMouseLeave();
	if (child) child->OnMouseEnter();
	hovered_child = child;
}
bool CBaseContainer::TryFocusOn(IWidget* child) {
	if (focused_child != child) {
		if (child){
			if (focused_child) focused_child->OnFocusLose();
			focused_child = child;
			return child->TryFocusGain();
		}else{
			if(can_focus_on_nothing){
				if (focused_child) focused_child->OnFocusLose();
				focused_child = nullptr;
				//You can focus on nothing if you want to
				//(as proved in elementary school)
				return true;
			}else{
				return false;
			}
		}
	}else{
		//TODO: Consider this.. Should it return true or false?
		//Probably doesn't matter, I can't imagine the mouse will care.
		return false;
	}
}
void CBaseContainer::PressOn(IWidget* child) {
	pressed_child = child;
	if (child) {
		TryFocusOn(child);
		child->OnMousePress();
	} else TryFocusOn(0);
}

}}

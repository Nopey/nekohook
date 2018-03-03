#include "menubutton.hpp"
#include "../../framework/input.hpp"
#include "../../framework/drawing.hpp"
#include "../gui.hpp"

namespace gui { namespace menu {
    CMenuButton::CMenuButton(const char * str):CBaseWidget(str){
    }
    void CMenuButton::Draw(){
        auto abs = AbsolutePosition();
        auto color=focus?(child_focused?colors::pink:colors::blue):colors::green;
    	draw::RectFilled(abs.first, abs.second, size.first, size.second, colors::Transparent(color, 0.3));
    	draw::Rect(abs.first, abs.second, size.first, size.second, color);
    }
    void CMenuButton::Update(){
    }
// User input functions
void CMenuButton::OnMouseEnter() {
	child->Show();
}

void CMenuButton::OnMouseLeave() {
	if(!focus) child->Hide();
}

bool CMenuButton::TryFocusGain() {
    child->Show();
	return true;
}
void CMenuButton::OnFocusLose() {
    if(child&&child_focused) child->OnFocusLose();
    if(child) child->Hide();
	child_focused=false;
	CBaseWidget::OnFocusLose();
}
void CMenuButton::OnKeyPress(int key) {
    if (child && child_focused){
		child->OnKeyPress(key);
	}else{
		if (key==backkey.value){
            if(child&&child_focused) child->OnFocusLose();
			child_focused=false;
		}else if (key==activatekey.value){
            child_focused|=(child&&!child_focused&&child->TryFocusGain());
		}
		
	}
}
void CMenuButton::OnKeyRelease(int key) {
	//Tempted to just fully hand these key release events on, but that'll just slow things down.
	if (child && child_focused) child->OnKeyRelease(key);
}
bool CMenuButton::ConsumesKey(int key) {
    //We need a child to consume anything.
	//If our child consumes, we can't. If we have a child who doesn't consume we can defocus them
	return child &&(child_focused && (child->ConsumesKey(key) || key==backkey)
	//If we aren't focusing on our child we can focus on our child.
	|| ((!child_focused) && key==activatekey));
}


    // Visiblity
    void CMenuButton::Hide() {
        CBaseWidget::Hide();
        if(child) child->Hide();
    }
    // Children
    void CMenuButton::SetChild(IWidget* child) {
        this->child=child;
    	child->parent = this;
    }
}}
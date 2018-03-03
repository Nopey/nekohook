
/*
 *
 * 	This is the menu constructor for the gui. This build a menu our of elements for us to use.
 *
 */

#include "../../util/catvars.hpp"	// CatMenuRoot
#include "../../util/logging.hpp"
#include "../gui.hpp"

#include "menu.hpp"
#include "menubutton.hpp"
#include <sstream>

// menus aspect ratio, dont judge
#define aspectx 13
#define aspecty 7

namespace gui { namespace menu {

CMenu pMenu("Menu Root");

void populateMenu(CMenu *menu, CatMenuTree &branch, unsigned int depth=0){
  g_CatLogging.log(" cc %d c %d",branch.cat_children.size(),branch.children.size());
  for(auto child:branch.cat_children){
    g_CatLogging.log(" + %s", child->name.c_str());
    CMenuButton *button=new CMenuButton(child->name.c_str());
    button->size = std::make_pair(30, 40);
    menu->AddChild(button);
  }
  for(auto& child:branch.children){
    g_CatLogging.log("[ %s", child.name.c_str());
    CMenuButton *button=new CMenuButton(child.name.c_str());
    button->size = std::make_pair(30, 40);
    menu->AddChild(button);
    CMenu *childMenu = new CMenu(child.name.c_str());
    childMenu->position_mode = base::FLOATING;
    childMenu->size = std::make_pair(200, 400);
    button->SetChild(childMenu);
    populateMenu(childMenu, child, depth+1);
    g_CatLogging.log("] %s", child.name.c_str());
  }
}
CMenu::CMenu(const char * str):CBaseWindow(str){}

void CMenu::Update() {
  offset = std::make_pair(parent->size.first,0);
  if(focus){

  }
  CBaseWindow::Update();
}

void Init() {
  pMenu.position_mode = base::FLOATING;
  pMenu.size = std::make_pair(200, 400);
  g_pGui.AddChild(&pMenu);
  populateMenu(&pMenu,CatMenuRoot);
  
  std::stringstream ss;
  CatMenuRoot.Stringify(ss);
  g_CatLogging.log("%s",ss.str().c_str());
}

}}

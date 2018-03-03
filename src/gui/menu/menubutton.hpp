#pragma once
#include "../base/CBaseWidget.hpp"
#include <vector>

namespace gui { namespace menu {
class CMenuButton : public base::CBaseWidget {
public:
  CMenuButton(const char *);
  std::vector<int> menu_position = {0};
  virtual void OnMouseEnter();
  virtual void OnMouseLeave();
  virtual bool TryFocusGain();
  virtual void OnFocusLose();
  virtual void OnKeyPress(int);
  virtual void OnKeyRelease(int);
  virtual bool ConsumesKey(int);
  virtual void Hide();
  //virtual void OnMousePress();
  virtual void Draw();
  virtual void Update();

  void SetChild(IWidget *);
  bool child_focused=false;
  IWidget* child;
};
}}
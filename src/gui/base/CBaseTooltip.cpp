
/*
 *
 *  An easy way to display information to the user.
 *  Make sure to set max_size to have clamping
 *
 */

#include "../../util/stringhelpers.hpp"
#include "../../util/colors.hpp"
#include "../../framework/input.hpp" // For mouse location
#include "../../framework/drawing.hpp"

#include "CBaseTooltip.hpp"

// Draw tooltip on mouse, and put size and offset somewhere else to prevent child by point from going off
// Make sure the root hover on works, and that it will recurse
namespace gui { namespace base {

CBaseTooltip::CBaseTooltip(const char* _name) : CBaseWidget(_name) {
  position_mode = FLOATING; // We dont want anything to move us
  // Here we want to set the size and offset to something that ChildByPoint wont find
  offset = std::make_pair(-128, -128);
  size = std::make_pair(0, 0);
}

// as well as set our zindex to always be on top
void CBaseTooltip::Update() {
  zindex = -100;
}

void CBaseTooltip::Draw() {

  // We cant draw an empty string
  if (!parent || parent->GetTooltip().empty()) return;
  tooltip = parent->GetTooltip();

  // Get tooltip size
  auto tooltip_size = draw::GetStringLength(tooltip.c_str(), 1, 20);

  // Check if we need to wrap
  if (tooltip_size.first > max_size.first) {

    std::string wrapped_str; // A place to store the wrapped string, with the size ;)
    std::string cur_line; // Our current line

    // Seperate our strings to make things easy
    auto sep_strs = sepstr(tooltip);

    // Here we take each word and make it fit in our box.
    for (const auto& cur_str : sep_strs) {

      // If it goes over max width, we add the current line to the ourput, then return
      if (draw::GetStringLength((cur_line + ' ' + cur_str).c_str(), 1, 20).first > max_size.first) {
        // Add the current line
        wrapped_str += cur_line + '\n';

        // Set the new line to our current string
        cur_line = cur_str;

        // continue to the next string
        continue;
      }

      // Add the current string to the line
      cur_line += ' ' + cur_str;
    }
    // Here we can set the new size and stuff.
    tooltip_size = draw::GetStringLength(wrapped_str.c_str(), 1, 20);
    tooltip = wrapped_str;
  }
  // Draw string
  draw::RectFilled(input::mouse.first, input::mouse.second, tooltip_size.first + 4, tooltip_size.second + 4, colors::Transparent(colors::black));
  draw::Rect(input::mouse.first, input::mouse.second, tooltip_size.first + 4, tooltip_size.second + 4, colors::pink);
  draw::String(tooltip.c_str(), input::mouse.first + 2, input::mouse.second + 2, 1, 20, colors::white);
}

}}
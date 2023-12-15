#include "icon_button.h"


UnfocusedState::UnfocusedState(IconButtonContext* context)  { context->SetBitmap(context->getUnfocusedIcon()); }
void UnfocusedState::focus(IconButtonContext* context)      { context->setState(std::make_unique<FocusedState>(context)); }


FocusedState::FocusedState(IconButtonContext* context)      { context->SetBitmap(context->getFocusedIcon()); }
void FocusedState::unfocus(IconButtonContext* context)      { context->setState(std::make_unique<UnfocusedState>(context)); }


IconButtonContext::IconButtonContext(const wxBitmap& unfocused, const wxBitmap& focused,
    wxWindow* parent, const wxPoint& pos, const wxSize& size) :
    wxButton(parent, wxID_ANY, wxEmptyString, pos, size, wxBORDER_NONE), _unfocused_icon(unfocused), _focused_icon(focused)
{
    SetBackgroundColour(parent->GetBackgroundColour());
    _state = std::make_unique<UnfocusedState>(this);
}

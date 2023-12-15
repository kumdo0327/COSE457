#ifndef __ICON_BUTTON_H__
#define __ICON_BUTTON_H__

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


class IconButtonContext;
class IconButtonState
{
public:
    virtual ~IconButtonState() = default;
    virtual void focus(IconButtonContext* context) = 0;
    virtual void unfocus(IconButtonContext* context) = 0;
};


class UnfocusedState : public IconButtonState
{
public:
    UnfocusedState(IconButtonContext* context);
    void focus(IconButtonContext* context) override;
    void unfocus(IconButtonContext* context) override {}
};


class FocusedState : public IconButtonState
{
public:
    FocusedState(IconButtonContext* context);
    void focus(IconButtonContext* context) override {}
    void unfocus(IconButtonContext* context) override;
};




class IconButtonContext : public wxButton
{
public:
    IconButtonContext(const wxBitmap& unfocused, const wxBitmap& focused,
        wxWindow* parent, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);

    void setState(std::unique_ptr<IconButtonState> state)   { _state = std::move(state); }
    const wxBitmap& getUnfocusedIcon() const                { return _unfocused_icon; }
    const wxBitmap& getFocusedIcon() const                  { return _focused_icon; }

    void unfocus()  { _state->unfocus(this); }
    void focus()    { _state->focus(this); }

private:
    std::unique_ptr<IconButtonState> _state;
    const wxBitmap _unfocused_icon;
    const wxBitmap _focused_icon;
};
#endif

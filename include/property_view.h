#ifndef __PROPERTY_VIEW_H__
#define __PROPERTY_VIEW_H__

#include <array>

#include <wx/textctrl.h>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


class PropertyView : public wxPanel
{
private:
    std::array<wxTextCtrl*, 7> _ctrls;
public:
    typedef struct { float r, g, b; } RGB;
    using const_iterator = decltype(_ctrls)::const_iterator;

public:
    PropertyView(wxWindow* parent);

    virtual void setData(RGB rgb, float x, float y, float width, float height);

    constexpr const_iterator cbegin() const noexcept    { return _ctrls.cbegin(); }
    constexpr const_iterator cend() const noexcept      { return _ctrls.cend();  }
    wxButton* getForwardButton() const                  { return _bring_forward; }
    wxButton* getBackwardButton() const                 { return _send_backward; }
    
    virtual float getRed() const                        { return toFloat(_r); }
    virtual float getGreen() const                      { return toFloat(_g); }
    virtual float getBlue() const                       { return toFloat(_b); }
    virtual float getX() const                          { return toFloat(_x); }
    virtual float getY() const                          { return toFloat(_y); }
    virtual float getWidth() const                      { return toFloat(_width); }
    virtual float getHeight() const                     { return toFloat(_height); }

protected:
    static constexpr int posY(int y, int interval, int iter)    { return y + interval * iter; }
    void setTextCtrlSize(int width, int height)                 { for (auto ctrl : _ctrls) ctrl->SetSize(width, height); }
    void setTextCtrlInterval(int x, int y, int interval)        { for (int i = 0; i != 7; i++) _ctrls[i]->SetPosition(wxPoint(x, posY(y, interval, i))); }
    void setButtonSize(int width, int height)                   { _bring_forward->SetSize(width, height); _send_backward->SetSize(width, height); }
    void setForwardPosition(int x, int y)                       { _bring_forward->SetPosition(wxPoint(x, y)); }
    void setBackwardPosition(int x, int y)                      { _send_backward->SetPosition(wxPoint(x, y)); }

private:
    static float toFloat(const wxTextCtrl* ctrl) { return (float)std::atof(ctrl->GetValue().c_str()); }

private:
    wxTextCtrl* _r;
    wxTextCtrl* _g;
    wxTextCtrl* _b;
    wxTextCtrl* _x;
    wxTextCtrl* _y;
    wxTextCtrl* _width;
    wxTextCtrl* _height;

    wxButton* _bring_forward; // Bring Forward
    wxButton* _send_backward; // Send Backward
};




class DefaultProperty : public PropertyView
{
public:
    DefaultProperty(wxWindow* parent);

protected:
    static constexpr int DEFAULT_WIDTH = 128;
    static constexpr int DEFAULT_HEIGHT = 24;
    static constexpr int DEFAULT_X = 16;
    static constexpr int DEFAULT_Y = 32;
    static constexpr int DEFAULT_INTERVAL = 64;
    static constexpr int DEFAULT_LABEL_X = DEFAULT_X;
    static constexpr int DEFAULT_LABEL_Y = 16;
    const wxColour DEFAULT_LABEL_COLOR = wxColour(0xFF, 0xFF, 0xFF);

    static constexpr int DEFAULT_BUTTON_WIDTH = DEFAULT_WIDTH;
    static constexpr int DEFAULT_BUTTON_HEIGHT = DEFAULT_HEIGHT;
    static constexpr int DEFAULT_BTN1_Y = posY(DEFAULT_LABEL_Y, DEFAULT_INTERVAL, 7);
    static constexpr int DEFAULT_BTN2_Y = DEFAULT_BTN1_Y + DEFAULT_BUTTON_HEIGHT + 16;

protected:
    void setLabel(int x, int y, int interval, std::initializer_list<std::string> labels);
};




class LineProperty : public DefaultProperty
{
public:
    LineProperty(wxWindow* parent);

public:
    void setData(RGB rgb, float x, float y, float width, float height) override
                                        { PropertyView::setData(rgb, x, y, x + width, y + height); }
    float getWidth() const override     { return PropertyView::getWidth() - PropertyView::getX(); }
    float getHeight() const override    { return PropertyView::getHeight() - PropertyView::getY(); }
};


class RecProperty : public DefaultProperty
{
public:
    RecProperty(wxWindow* parent);
};


class ElpsProperty : public DefaultProperty
{
public:
    ElpsProperty(wxWindow* parent);

public:
    void setData(RGB rgb, float x, float y, float width, float height) override
                                        { PropertyView::setData(rgb, x, y, width / 2.0f, height / 2.0f); }
    float getWidth() const override     { return PropertyView::getWidth() * 2.0f; }
    float getHeight() const override    { return PropertyView::getHeight() * 2.0f; }
};
#endif

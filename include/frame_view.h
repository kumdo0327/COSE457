#ifndef __FRAME_VIEW_H__
#define __FRAME_VIEW_H__

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "canvas_view.h"
#include "panel_view.h"
#include "property_view.h"


class FrameView
{
public:
    static wxSize getDefaultSize() { return wxSize(_MIN_WIDTH + 256, _MIN_HEIGHT + 128); }
public:
    FrameView(wxWindow* frame);

    void setCanvasPos();
    void setPanelPos();
    void setPropertyPos();
    void setViewPos()                           { setCanvasPos(); setPanelPos(); setPropertyPos(); }
    void setViewPos(wxSizeEvent&)               { setViewPos(); }
    void setProperty(PropertyView* property)    { if (_property) delete _property; _property = property; setPropertyPos(); }

    PanelView* getPanel()                       { return _panel; }
    CanvasView* getCanvas()                     { return _canvas; }
    PropertyView* getProperty()                 { return _property; }

private:
    static constexpr int _PANEL_WIDTH = 96;
    static constexpr int _PANEL_ICON = 64;
    static constexpr int _PROPERTY_WIDTH = 180;
    static constexpr int _BORDER = 16;
    static constexpr int _2xBORDER = _BORDER * 2;
    static constexpr int _MIN_HEIGHT = _PANEL_WIDTH * 4;
    static constexpr int _MIN_CANVAS_WIDTH = 32;
    static constexpr int _MIN_WIDTH = _PANEL_WIDTH + _MIN_CANVAS_WIDTH + _2xBORDER + _PROPERTY_WIDTH;

private:
    wxWindow* _frame;
    CanvasView* _canvas;
    PanelView* _panel;
    PropertyView* _property;
};
#endif

#include "frame_view.h"


FrameView::FrameView(wxWindow* frame) : _property(nullptr)
{
    frame->SetBackgroundColour(wxColour(0x1F, 0x1F, 0x1F));
    frame->SetMinSize(frame->ClientToWindowSize(wxSize(_MIN_WIDTH, _MIN_HEIGHT)));
    frame->SetSize(frame->ClientToWindowSize(wxSize(_MIN_WIDTH + 256, _MIN_HEIGHT + 160)));

    _frame = frame;
    _canvas = new CanvasView(frame);
    _panel = new PanelView(frame, _PANEL_WIDTH, _PANEL_ICON);

    setViewPos();
}


void FrameView::setCanvasPos()
{
    // width : height = 4 : 3
    auto width = _frame->GetClientSize().GetWidth() - _PANEL_WIDTH - _PROPERTY_WIDTH - _2xBORDER;
    auto height = _frame->GetClientSize().GetHeight() - _2xBORDER;

    if (width * 3 > height << 2) {

        width = (height << 2) / 3;
        _canvas->SetPosition(wxPoint(_PANEL_WIDTH + \
                ((_frame->GetClientSize().GetWidth() - _PANEL_WIDTH - _PROPERTY_WIDTH - width) >> 1), _BORDER));
    }
    else {

        height = width * 3 >> 2;
        _canvas->SetPosition(wxPoint(_PANEL_WIDTH + _BORDER, (_frame->GetClientSize().GetHeight() - height) >> 1));
    }
    _canvas->SetSize(wxSize(width, height));
}


void FrameView::setPanelPos()
{
    _panel->SetPosition(wxPoint(0, 0));
    _panel->SetSize(wxSize(_PANEL_WIDTH, _frame->GetClientSize().GetHeight()));
}


void FrameView::setPropertyPos()
{
    if (!_property)
        return;
    _property->SetPosition(wxPoint(_frame->GetClientSize().GetWidth() - _PROPERTY_WIDTH, 0));
    _property->SetSize(wxSize(_PROPERTY_WIDTH, _frame->GetClientSize().GetHeight()));
}

#include "property_view.h"


PropertyView::PropertyView(wxWindow* parent) : wxPanel(parent)
{
    SetBackgroundColour(parent->GetBackgroundColour());

    for (auto& ctrl : _ctrls)
        ctrl = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    _r = _ctrls[0];
    _g = _ctrls[1];
    _b = _ctrls[2];
    _x = _ctrls[3];
    _y = _ctrls[4];
    _width = _ctrls[5];
    _height = _ctrls[6];

    _bring_forward = new wxButton(this, wxID_ANY, "Bring Forward");
    _send_backward = new wxButton(this, wxID_ANY, "Send Backward");
}


void PropertyView::setData(RGB rgb, float x, float y, float width, float height)
{
    _r->SetValue(std::to_string(rgb.r));
    _g->SetValue(std::to_string(rgb.g));
    _b->SetValue(std::to_string(rgb.b));
    _x->SetValue(std::to_string(x));
    _y->SetValue(std::to_string(y));
    _width->SetValue(std::to_string(width));
    _height->SetValue(std::to_string(height));
}


DefaultProperty::DefaultProperty(wxWindow* parent) : PropertyView(parent)
{
    setTextCtrlSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    setTextCtrlInterval(DEFAULT_X, DEFAULT_Y, DEFAULT_INTERVAL);
    setButtonSize(DEFAULT_BUTTON_WIDTH, DEFAULT_BUTTON_HEIGHT);
    setForwardPosition(DEFAULT_X, DEFAULT_BTN1_Y);
    setBackwardPosition(DEFAULT_X, DEFAULT_BTN2_Y);
}


void DefaultProperty::setLabel(int x, int y, int interval, std::initializer_list<std::string> labels)
{
    wxStaticText* stexts[7];
    
    size_t i = 0;
    for (auto& label : labels) {

        stexts[i] = new wxStaticText(this, wxID_ANY, label.c_str(), wxPoint(DEFAULT_LABEL_X, DEFAULT_LABEL_Y + DEFAULT_INTERVAL * i));
        stexts[i]->SetForegroundColour(DEFAULT_LABEL_COLOR);
        ++i;
    }
}


LineProperty::LineProperty(wxWindow* parent) : DefaultProperty(parent)
{
    setLabel(DEFAULT_LABEL_X, DEFAULT_LABEL_Y, DEFAULT_INTERVAL, {"Red", "Green", "Blue", "X", "Y", "endX", "endY"});
}


RecProperty::RecProperty(wxWindow* parent) : DefaultProperty(parent)
{
    setLabel(DEFAULT_LABEL_X, DEFAULT_LABEL_Y, DEFAULT_INTERVAL, {"Red", "Green", "Blue", "X", "Y", "Width", "Height"});
}


ElpsProperty::ElpsProperty(wxWindow* parent) : DefaultProperty(parent)
{
    setLabel(DEFAULT_LABEL_X, DEFAULT_LABEL_Y, DEFAULT_INTERVAL, {"Red", "Green", "Blue", "X", "Y", "Rx", "Ry"});
}

#include "panel_view.h"


PanelView::PanelView(wxWindow* parent, int width, int icon_size) : wxPanel(parent), EXE_DIR(fs::path(std::string(wxStandardPaths::Get().GetExecutablePath().mb_str())).parent_path())
{
    SetBackgroundColour(parent->GetBackgroundColour());

    _selection = new IconButtonContext(loadImg(path_cursor, width, icon_size), loadImg(path_cursor_on, width, icon_size),
        this, wxPoint(0, 0), wxSize(width, width));
    _line = new IconButtonContext(loadImg(path_line, width, icon_size), loadImg(path_line_on, width, icon_size),
        this, wxPoint(0, width), wxSize(width, width));
    _rec = new IconButtonContext(loadImg(path_rec, width, icon_size), loadImg(path_rec_on, width, icon_size),
        this, wxPoint(0, width * 2), wxSize(width, width));
    _elps = new IconButtonContext(loadImg(path_elps, width, icon_size), loadImg(path_elps_on, width, icon_size),
        this, wxPoint(0, width * 3), wxSize(width, width));

    _buttons[0] = _selection;
    _buttons[1] = _line;
    _buttons[2] = _rec;
    _buttons[3] = _elps;

    _selection->Bind(wxEVT_BUTTON, &PanelView::onSelectionButton, this);
    _line->Bind(wxEVT_BUTTON, &PanelView::onLineButton, this);
    _rec->Bind(wxEVT_BUTTON, &PanelView::onRecButton, this);
    _elps->Bind(wxEVT_BUTTON, &PanelView::onElpsButton, this);

    _selection->focus();
}


wxImage PanelView::loadImg(const char path[], int width, int icon_size) const
{
    wxImage img((EXE_DIR / path).c_str(), wxBITMAP_TYPE_PNG);
    img.Rescale(icon_size, icon_size, wxIMAGE_QUALITY_NEAREST);

    wxImage ret(width, width);
    ret.SetAlpha((unsigned char*)std::calloc(width * width, sizeof(unsigned char)));
    ret.Paste(img, (width - icon_size) >> 1, (width - icon_size) >> 1);
    return ret;
}

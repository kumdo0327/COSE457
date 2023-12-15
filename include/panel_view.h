#ifndef __PANEL_VIEW_H__
#define __PANEL_VIEW_H__

#include <array>
#include <filesystem>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/filename.h>
#include <wx/stdpaths.h>

#include "icon_button.h"

namespace fs = std::filesystem;


class PanelView : public wxPanel
{
public:
    PanelView(wxWindow* parent, int width, int icon_size);

    IconButtonContext* getSelectionButton() { return _selection; }
    IconButtonContext* getLineButton()      { return _line; }
    IconButtonContext* getRecButton()       { return _rec; }
    IconButtonContext* getElpsButton()      { return _elps; }

private:
    static constexpr char path_cursor_on[] = "img/cursor-on.png";
    static constexpr char path_cursor[] = "img/cursor.png";
    static constexpr char path_line_on[] = "img/diagonal-line-on.png";
    static constexpr char path_line[] = "img/diagonal-line.png";
    static constexpr char path_rec_on[] = "img/rectangle-on.png";
    static constexpr char path_rec[] = "img/rectangle.png";
    static constexpr char path_elps_on[] = "img/ellipse-on.png";
    static constexpr char path_elps[] = "img/ellipse.png";
    const fs::path EXE_DIR;

private:// Function
    wxImage loadImg(const char path[], int width, int icon_size) const;
    void setUnfocused() { for (auto ptr : _buttons) ptr->unfocus(); }

private:// EventHandler
    void onSelectionButton(wxCommandEvent& event)   { setUnfocused(); _selection->focus(); event.Skip(); }
    void onLineButton(wxCommandEvent& event)        { setUnfocused(); _line->focus(); event.Skip(); }
    void onRecButton(wxCommandEvent& event)         { setUnfocused(); _rec->focus(); event.Skip(); }
    void onElpsButton(wxCommandEvent& event)        { setUnfocused(); _elps->focus(); event.Skip(); }

private:
    std::array<IconButtonContext*, 4> _buttons;
    IconButtonContext* _selection;
    IconButtonContext* _line;
    IconButtonContext* _rec;
    IconButtonContext* _elps;
};
#endif

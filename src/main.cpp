// wxWidgets "Hello World" Program available on https://docs.wxwidgets.org/trunk/overview_helloworld.html

// For compilers that support precompilation, includes "wx/wx.h".
#include "controller.h"

class App : public wxApp 
{
public:
    virtual bool OnInit(); 
}; 

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    wxImage::AddHandler(new wxPNGHandler);
    Controller* controller = new Controller(this);
    controller->Show(true);
    return true;
}

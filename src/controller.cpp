#include "controller.h"


// Initialization
/////////////////
Controller::Controller(wxApp* app) :
    wxFrame(nullptr, wxID_ANY, "COSE457", wxDefaultPosition, FrameView::getDefaultSize()), _frame(new FrameView(this)),
    _mouse_context(_frame->getCanvas())
{
    initSingleton();
    Bind(wxEVT_SIZE, &FrameView::setViewPos, _frame);
    bindPanelButton();
    bindCanvas();
    bindFrame(app);
}


void Controller::initSingleton()
{
    Observers::getInstance().init(_frame);
    _frame->getCanvas()->pushFrontToPipeline(&RenderingQueue::getInstance());
}


void Controller::bindPanelButton()
{
    auto panel = _frame->getPanel();
    panel->getSelectionButton()->Bind(wxEVT_BUTTON, &Controller::onSelection, this);
    panel->getLineButton()->Bind(wxEVT_BUTTON, &Controller::onLine, this);
    panel->getRecButton()->Bind(wxEVT_BUTTON, &Controller::onRec, this);
    panel->getElpsButton()->Bind(wxEVT_BUTTON, &Controller::onElps, this);
}


void Controller::bindCanvas()
{
    auto canvas = _frame->getCanvas();
    canvas->Bind(wxEVT_LEFT_DOWN, &Controller::onLeftDown, this);
    canvas->Bind(wxEVT_LEFT_UP, &Controller::onLeftUp, this);
    canvas->Bind(wxEVT_MOTION, &Controller::onMotion, this);
    canvas->Bind(wxEVT_ENTER_WINDOW, &Controller::onEnterWindow, this);
    canvas->Bind(wxEVT_LEAVE_WINDOW, &Controller::onLeaveWindow, this);
    canvas->Bind(wxEVT_SIZE, &Controller::onSize, this);
}




// Function
///////////
void Controller::postprocessCommand()
{
    auto selection = ObjectContainer::getInstance().getSelection();
    if (selection) {

        auto property = _frame->getProperty();
        if (property) {

            for (auto iter = property->cbegin(); iter != property->cend(); iter++)
                (*iter)->Bind(wxEVT_TEXT_ENTER, &Controller::onInput, this);
            property->getForwardButton()->Bind(wxEVT_BUTTON, &Controller::onForward, this);
            property->getBackwardButton()->Bind(wxEVT_BUTTON, &Controller::onBackward, this);
            selection->notify();
        }
    }
    else
        _frame->setProperty(nullptr);
}




// Property EventHandler
////////////////////////
void Controller::onInput(wxCommandEvent&)
{
    auto ptr = _frame->getProperty();
    ObjectContainer::getInstance().setSelection(CanvasObject::RGB{ptr->getRed(), ptr->getGreen(), ptr->getBlue()},
                                                CanvasObject::Coord{ptr->getX(), ptr->getY()}, CanvasObject::Size{ptr->getWidth(), ptr->getHeight()});
}




// Frame EventHandler
/////////////////////
void Controller::onKey(wxKeyEvent& event)
{
    if (_frame->getCanvas()->HasFocus()) {

        KeyContext key_context(_frame);
        if (event.ShiftDown())
            key_context.shift();

        switch (event.GetKeyCode()) {

        case WXK_BACK:
            key_context.back();
            break;
        
        case WXK_LEFT:
            key_context.left();
            break;
        case WXK_UP:
            key_context.up();
            break;
        case WXK_RIGHT:
            key_context.right();
            break;
        case WXK_DOWN:
            key_context.down();
            break;
        }
    }
    event.Skip();
}

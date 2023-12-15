#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <unordered_map>
#include <cstdlib>

#include "mouse_state.h"
#include "key_state.h"


class Controller : public wxFrame
{
public:
    Controller(wxApp* app);

private:// Initialization
    void initSingleton();
    void bindCanvas();
    void bindFrame(wxApp* app)              { app->Bind(wxEVT_KEY_DOWN, &Controller::onKey, this); }
    void bindPanelButton();

private:// Function
    void postprocessCommand();

private:// Functor
    class CmdPostProcessor {
    public:
        CmdPostProcessor(Controller* frame) : _frame(frame) {}
        void operator()() const { _frame->postprocessCommand(); }
    private:
        Controller* _frame;
    };

private:// Panel EventHandler
    void onSelection(wxCommandEvent& event) { _command = std::make_unique<SelectionHandler<CmdPostProcessor>>(this, _frame, CmdPostProcessor(this)); event.Skip(); }
    void onLine(wxCommandEvent& event)      { _command = std::make_unique<LineObjHandler<CmdPostProcessor>>(this, _frame, CmdPostProcessor(this)); event.Skip(); }
    void onRec(wxCommandEvent& event)       { _command = std::make_unique<RecObjHandler<CmdPostProcessor>>(this, _frame, CmdPostProcessor(this)); event.Skip(); }
    void onElps(wxCommandEvent& event)      { _command = std::make_unique<ElpsObjHandler<CmdPostProcessor>>(this, _frame, CmdPostProcessor(this)); event.Skip(); }

private:// Canvas EventHandler
    void onSize(wxSizeEvent&)               { _frame->getCanvas()->render(); }
    void onLeftDown(wxMouseEvent& event)    { _mouse_context.onLeftDown(event.GetPosition().x, event.GetPosition().y, _command.get()); event.Skip(); }
    void onLeftUp(wxMouseEvent& event)      { _mouse_context.onLeftUp(event.GetPosition().x, event.GetPosition().y, _command.get()); event.Skip(); }
    void onMotion(wxMouseEvent& event)      { _mouse_context.onMotion(event.GetPosition().x, event.GetPosition().y, _command.get()); event.Skip(); }
    void onEnterWindow(wxMouseEvent& event) { _mouse_context.onEnterWindow(); event.Skip(); }
    void onLeaveWindow(wxMouseEvent& event) { _mouse_context.onLeaveWindow(); event.Skip(); }

private:// Property EventHandler
    void onInput(wxCommandEvent&);
    void onForward(wxCommandEvent&)         { auto& ref = ObjectContainer::getInstance(); ref.bringForward(ref.getSelection()); }
    void onBackward(wxCommandEvent&)        { auto& ref = ObjectContainer::getInstance(); ref.sendBackward(ref.getSelection()); }

private:// Frame EventHandler
    void onKey(wxKeyEvent& event);

private:
    FrameView* _frame;
    std::unique_ptr<AreaCmdBase> _command;
    MouseContext _mouse_context;
};

#endif

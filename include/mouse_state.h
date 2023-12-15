#ifndef __MOUSE_STATE_H__
#define __MOUSE_STATE_H__

#include "area_command.h"
#include "mouse_action_overlay.h"


class MouseContext;
class MouseStateBase
{
public:
    virtual ~MouseStateBase() = default;
    virtual void onLeftDown(MouseContext* context, int x, int y, float inv_zoom, AreaCmdBase* cmd) = 0;
    virtual void onLeftUp(MouseContext* context, int x, int y, float inv_zoom, AreaCmdBase* cmd) = 0;
    virtual void onMotion(MouseContext* context, int x, int y, float inv_zoom, AreaCmdBase* cmd) = 0;
    virtual void onLeaveWindow(MouseContext* context);
    virtual void onEnterWindow(MouseContext* context);
};


class LeftUpState : public MouseStateBase
{
public:
    void onLeftDown(MouseContext* context, int x, int y, float inv_zoom, AreaCmdBase* cmd) override;
    void onLeftUp(MouseContext* context, int x, int y, float inv_zoom, AreaCmdBase* cmd) override {}
    void onMotion(MouseContext* context, int x, int y, float inv_zoom, AreaCmdBase* cmd) override {}
    void onEnterWindow(MouseContext* context) override {}
};


class LeftDownState : public MouseStateBase
{
public:
    void onLeftDown(MouseContext* context, int x, int y, float inv_zoom, AreaCmdBase* cmd) override {}
    void onLeftUp(MouseContext* context, int x, int y, float inv_zoom, AreaCmdBase* cmd) override;
    void onMotion(MouseContext* context, int x, int y, float inv_zoom, AreaCmdBase* cmd) override;
};


class DraggingState : public MouseStateBase
{
public:
    void onLeftDown(MouseContext* context, int x, int y, float inv_zoom, AreaCmdBase* cmd) override {}
    void onLeftUp(MouseContext* context, int x, int y, float inv_zoom, AreaCmdBase* cmd) override;
    void onMotion(MouseContext* context, int x, int y, float inv_zoom, AreaCmdBase* cmd) override;
};


class LeavingState : public MouseStateBase
{
    void onLeftDown(MouseContext* context, int x, int y, float inv_zoom, AreaCmdBase* cmd) override {}
    void onLeftUp(MouseContext* context, int x, int y, float inv_zoom, AreaCmdBase* cmd) override {}
    void onMotion(MouseContext* context, int x, int y, float inv_zoom, AreaCmdBase* cmd) override {}
    void onLeaveWindow(MouseContext* context) override {}
};


class MouseContext
{
public:
    MouseContext(CanvasView* canvas) : _state(std::make_unique<LeftUpState>()), _canvas(canvas) {}

    void setState(std::unique_ptr<MouseStateBase> state)    { _state = std::move(state); }
    void setPos(int x, int y)                               { _x = x; _y = y; }
    void setX(int x)                                        { _x = x; }
    void setY(int y)                                        { _y = y; }

    int getX() const                                        { return _x; }
    int getY() const                                        { return _y; }

    void newOverlay(float x1, float y1, float x2, float y2);
    void setOverlayEnd(float x2, float y2)                  { _overlay->setEnd(x2, y2); _canvas->render(); }
    void delOverlay();

public:
    void onLeftDown(int x, int y, AreaCmdBase* cmd)         { _state->onLeftDown(this, x, y, _canvas->getInvertedZoom(), cmd); }
    void onLeftUp(int x, int y, AreaCmdBase* cmd)           { _state->onLeftUp(this, x, y, _canvas->getInvertedZoom(), cmd); }
    void onMotion(int x, int y, AreaCmdBase* cmd)           { _state->onMotion(this, x, y, _canvas->getInvertedZoom(), cmd); }
    void onEnterWindow()                                    { _state->onEnterWindow(this); }
    void onLeaveWindow()                                    { _state->onLeaveWindow(this); }

private:
    std::unique_ptr<MouseStateBase> _state;
    int _x;
    int _y;

    CanvasView* _canvas;
    std::unique_ptr<MouseActionOverlay> _overlay;
};
#endif

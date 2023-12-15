#include "mouse_state.h"


void MouseStateBase::onEnterWindow(MouseContext* context)
{
    context->setState(std::make_unique<LeftUpState>());
}


void MouseStateBase::onLeaveWindow(MouseContext* context)
{
    context->setState(std::make_unique<LeavingState>());
    context->delOverlay();
}


void LeftUpState::onLeftDown(MouseContext* context, int x, int y, float inv_zoom, AreaCmdBase* cmd)
{
    context->setState(std::make_unique<LeftDownState>());
    context->setPos(x, y);
}


void LeftDownState::onLeftUp(MouseContext* context, int x, int y, float inv_zoom, AreaCmdBase* cmd)
{
    context->setState(std::make_unique<LeftUpState>());
    if (cmd)
        cmd->point(inv_zoom * x, inv_zoom * y);
}


void LeftDownState::onMotion(MouseContext* context, int x, int y, float inv_zoom, AreaCmdBase* cmd)
{
    context->setState(std::make_unique<DraggingState>());
    context->newOverlay(inv_zoom * context->getX(), inv_zoom * context->getY(), inv_zoom * x, inv_zoom * y);
}


void DraggingState::onLeftUp(MouseContext* context, int x, int y, float inv_zoom, AreaCmdBase* cmd)
{
    context->setState(std::make_unique<LeftUpState>());
    context->delOverlay();

    int x1 = context->getX(); int x2 = x;
    int y1 = context->getY(); int y2 = y;

    if (cmd) {

        if (-2 <= x2 - x1 && x2 - x1 <= 2 && -2 <= y2 - y1 && y2 - y1 <= 2)
            cmd->point(inv_zoom * x1, inv_zoom * y1);
        else
            cmd->region(inv_zoom * x1, inv_zoom * y1, inv_zoom * x2, inv_zoom * y2);
    }
}


void DraggingState::onMotion(MouseContext* context, int x, int y, float inv_zoom, AreaCmdBase* cmd)
{
    context->setOverlayEnd(inv_zoom * x, inv_zoom * y);
}


void MouseContext::newOverlay(float x1, float y1, float x2, float y2)
{
    _overlay = std::make_unique<MouseActionOverlay>(x1, y1);
    _overlay->setEnd(x2, y2);
    _canvas->pushBackToPipeline(_overlay.get());
    _canvas->render();
}


void MouseContext::delOverlay()
{
    if (_overlay) {

        _canvas->eraseRenderingProcessor(_overlay.get());
        _canvas->render();
        _overlay.release();
    }
}

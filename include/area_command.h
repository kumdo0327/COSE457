#ifndef __AREA_COMMAND_H__
#define __AREA_COMMAND_H__

#include "observers.h"


class AreaCmdBase
{
public:
    AreaCmdBase(wxWindow* frame, FrameView* frame_view) : frame(frame), frame_view(frame_view) {}
    virtual ~AreaCmdBase() = default;
    virtual const CanvasObject* point(float x, float y) const = 0;
    virtual const CanvasObject* region(float x1, float y1, float x2, float y2) const = 0;

protected:
    wxWindow* frame;
    FrameView* frame_view;
};




template <class PostProcess>
class SelectionHandler : public AreaCmdBase
{
public:
    SelectionHandler(wxWindow* frame, FrameView* frame_view, PostProcess process) : AreaCmdBase(frame, frame_view), _process(process) {}
    const CanvasObject* point(float x, float y) const override;
    const CanvasObject* region(float x1, float y1, float x2, float y2) const override;

private:
    PostProcess _process;
};




template <class Obj, class View, class PostProcess>
class ObjectHandler : public AreaCmdBase
{
public:
    ObjectHandler(wxWindow* frame, FrameView* frame_view, PostProcess process) : AreaCmdBase(frame, frame_view), _process(process) {}
    const CanvasObject* point(float x, float y) const override;

protected:
    virtual std::vector<const ObjectObserverBase*> getObs(Observers& obs) const = 0;
    const CanvasObject* postprocessObject(std::shared_ptr<CanvasObject> obj) const;

private:
    PostProcess _process;
};




template <class Obj, class View, class PostProcess>
class PolygonObjHandler : public ObjectHandler<Obj, View, PostProcess>
{
public:
    PolygonObjHandler(wxWindow* frame, FrameView* frame_view, PostProcess process) : ObjectHandler<Obj, View, PostProcess>(frame, frame_view, process) {}
    const CanvasObject* region(float x1, float y1, float x2, float y2) const override;
};




template <class PostProcess>
class LineObjHandler : public ObjectHandler<LineObject, LineView, PostProcess>
{
public:
    LineObjHandler(wxWindow* frame, FrameView* frame_view, PostProcess process) : ObjectHandler<LineObject, LineView, PostProcess>(frame, frame_view, process) {}
    const CanvasObject* region(float x1, float y1, float x2, float y2) const override;
protected:
    std::vector<const ObjectObserverBase*> getObs(Observers& obs) const override { return { obs.getLineSelectionUpdater() }; }
};


template <class PostProcess>
class RecObjHandler : public PolygonObjHandler<RecObject, RecView, PostProcess>
{
public:
    RecObjHandler(wxWindow* frame, FrameView* frame_view, PostProcess process) : PolygonObjHandler<RecObject, RecView, PostProcess>(frame, frame_view, process) {}
protected:
    std::vector<const ObjectObserverBase*> getObs(Observers& obs) const override { return { obs.getRecSelectionUpdater() }; }
};


template <class PostProcess>
class ElpsObjHandler : public PolygonObjHandler<ElpsObject, ElpsView, PostProcess>
{
public:
    ElpsObjHandler(wxWindow* frame, FrameView* frame_view, PostProcess process) : PolygonObjHandler<ElpsObject, ElpsView, PostProcess>(frame, frame_view, process) {}
protected:
    std::vector<const ObjectObserverBase*> getObs(Observers& obs) const override { return { obs.getElpsSelectionUpdater() }; }
};

#include "area_command.hpp"
#endif

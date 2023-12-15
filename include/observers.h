#ifndef __OBSERVERS_H__
#define __OBSERVERS_H__

#include "rendering_queue.h"
#include "frame_view.h"
#include "property_factory.h"


class Observers : public Singleton<Observers>
{
public:
    void init(FrameView* frame_view)                                { static auto temp = _init(frame_view); }
private:
    int _init(FrameView* frame_view);

public:
    const ObjectObserverBase* getCanvasUpdater() const              { return _canvas_updater; }
    const ObjectObserverBase* getPropertyUpdater() const            { return _property_updater; }

    const ObjectObserverBase* getCompositeSelectionUpdater() const  { return _composite_selection_updater; }
    const ObjectObserverBase* getLineSelectionUpdater() const       { return _line_selection_updater; }
    const ObjectObserverBase* getRecSelectionUpdater() const        { return _rec_selection_updater; }
    const ObjectObserverBase* getElpsSelectionUpdater() const       { return _elps_selection_updater; }

private:
    const ObjectObserverBase* _canvas_updater = nullptr;
    const ObjectObserverBase* _property_updater = nullptr;

    const ObjectObserverBase* _composite_selection_updater = nullptr;
    const ObjectObserverBase* _line_selection_updater = nullptr;
    const ObjectObserverBase* _rec_selection_updater = nullptr;
    const ObjectObserverBase* _elps_selection_updater = nullptr;
};
#endif

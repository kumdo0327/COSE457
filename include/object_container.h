#ifndef __OBJECT_CONTAINER_H__
#define __OBJECT_CONTAINER_H__

#include <list>
#include <limits>

#include "singleton.h"
#include "canvas_object.h"


class ObjectContainer : public Singleton<ObjectContainer>
{
public:
    using t = std::list<std::shared_ptr<CanvasObject>>;
public:
    CanvasObject* emplace(std::shared_ptr<CanvasObject> obj);
    void erase(const CanvasObject* obj);

    CanvasObject* select(float x, float y);
    CanvasObject* selectRegion(float left, float right, float bottom, float top, std::initializer_list<const ObjectObserverBase*> obs);
    CanvasObject* getSelection() const                      { return _selection.get(); }
    bool isSelectionAvailable() const                       { return _selection.get(); }

    void setSelection(CanvasObject::RGB rgb, CanvasObject::Coord coord, CanvasObject::Size size);
    void deselect()                                         { if(_selection) _selection->deselect(); _selection = nullptr; }
    void bringForward(CanvasObject* obj);
    void sendBackward(CanvasObject* obj);
    
    float normalizeZ(size_t z)                              { return (float)z / (_container.size() + 1); }
    float getMaxZ() const                                   { return _container.size(); }

private:
    t _container;
    std::shared_ptr<CanvasObject> _selection;
};
#endif

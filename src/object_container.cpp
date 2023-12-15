#include "object_container.h"


CanvasObject* ObjectContainer::emplace(std::shared_ptr<CanvasObject> obj)
{
    deselect();
    _selection = obj;

    auto ptr = _container.emplace_front(obj).get();
    ptr->setZ(_container.size());
    ptr->select();
    
    return ptr;
}


void ObjectContainer::erase(const CanvasObject* obj)
{
    if (!obj)
        return;
    auto z = obj->getZ();

    for (auto iter = _container.begin(); iter != _container.end(); ) {

        auto temp = iter;
        ++iter;

        if (temp->get()->getZ() == z)
            _container.erase(temp);
        else
            (*temp)->decreaseZ(z);
    }

    if (_selection.get() == obj)
        _selection = nullptr;
}


CanvasObject* ObjectContainer::select(float x, float y)
{
    deselect();
    size_t z = 0;

    for (auto& ptr : _container)
        if (ptr->hasPoint(x, y) && ptr->getZ() > z) {

            _selection = ptr;
            z = _selection->getZ();
        }

    if (_selection)
        _selection->select();

    return _selection.get();
}


CanvasObject* ObjectContainer::selectRegion(float left, float right, float bottom, float top, std::initializer_list<const ObjectObserverBase*> obs)
{
    deselect();
    
    std::list<std::shared_ptr<CanvasObject>> objs;
    for (auto& ptr : _container)
        if (ptr->inRegion(left, right, bottom, top))
            objs.push_back(ptr);

    // No selection
    if (objs.empty())
        return nullptr;
    
    // Single Selection
    if (objs.size() == 1) {

        (_selection = *objs.begin())->select();
        return _selection.get();
    }

    // Multi selection
    auto raw_composite = new CompositeObject(CanvasObject::DEFAULT_COORD, [this](const CanvasObject* obj){ this->erase(obj); });
    std::shared_ptr<std::remove_pointer_t<decltype(raw_composite)>> composite(raw_composite);

    composite->setZ(std::numeric_limits<size_t>::max());
    for (auto item : obs)
        composite->addObserver(item);
    composite->addObject(objs);
    (_selection = composite)->select();

    return _selection.get();
}


void ObjectContainer::setSelection(CanvasObject::RGB rgb, CanvasObject::Coord coord, CanvasObject::Size size)
{
    if (_selection) {

        _selection->silence();
        _selection->setRGB(rgb);
        _selection->setCoord(coord);
        _selection->sound();
        _selection->setSize(size);
    }
}


void ObjectContainer::bringForward(CanvasObject* obj)
{
    if (obj) {

        auto target = obj->getZ();
        for (auto& ptr : _container)
            ptr->decreaseZ(target);
        obj->setZ(getMaxZ());
    }
}


void ObjectContainer::sendBackward(CanvasObject* obj)
{
    if (obj) {

        auto target = obj->getZ();
        for (auto& ptr : _container)
            ptr->increaseZ(target);
        obj->setZ(1);
    }
}

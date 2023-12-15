
template <class PostProcess>
const CanvasObject* SelectionHandler<PostProcess>::point(float x, float y) const
{
    auto obj = ObjectContainer::getInstance().select(x, y);
    frame_view->setProperty(AbstractPropertyFactory::getInstance().createProperty(frame));
    _process();
    return obj;
}


template <class PostProcess>
const CanvasObject* SelectionHandler<PostProcess>::region(float x1, float y1, float x2, float y2) const
{
    auto& obs = Observers::getInstance();

    float left = x1 < x2 ? x1 : x2;
    float right = x1 < x2 ? x2 : x1;
    float bottom = y1 < y2 ? y1 : y2;
    float top = y1 < y2 ? y2 : y1;

    auto obj = ObjectContainer::getInstance().selectRegion(left, right, bottom, top, {obs.getCanvasUpdater(), obs.getCompositeSelectionUpdater()});
    frame_view->setProperty(AbstractPropertyFactory::getInstance().createProperty(frame));
    _process();
    return obj;
}


template <class Obj, class View, class PostProcess>
const CanvasObject* ObjectHandler<Obj, View, PostProcess>::point(float x, float y) const
{
    frame_view->setProperty(nullptr);
    std::shared_ptr<CanvasObject> obj = std::make_shared<Obj>(CanvasObject::Coord{x, y});
    RenderingQueue::getInstance().emplace(obj, std::make_unique<View>());
    return postprocessObject(obj);
}


template <class Obj, class View, class PostProcess>
const CanvasObject* ObjectHandler<Obj, View, PostProcess>::postprocessObject(std::shared_ptr<CanvasObject> obj) const
{
    auto& obs = Observers::getInstance();

    // Add observer
    auto ref = obj.get();
    ref->addObserver(obs.getCanvasUpdater());
    ref->addObserver(obs.getPropertyUpdater());
    for (auto ptr : getObs(obs))
        ref->addObserver(ptr);

    // Push to container
    ObjectContainer::getInstance().emplace(obj);
    frame_view->setProperty(AbstractPropertyFactory::getInstance().createProperty(frame));
    _process();
    return ref;
}


template <class Obj, class View, class PostProcess>
const CanvasObject* PolygonObjHandler<Obj, View, PostProcess>::region(float x1, float y1, float x2, float y2) const
{
    float left = x1 < x2 ? x1 : x2;
    float right = x1 < x2 ? x2 : x1;
    float bottom = y1 < y2 ? y1 : y2;
    float top = y1 < y2 ? y2 : y1;

    AreaCmdBase::frame_view->setProperty(nullptr);
    std::shared_ptr<CanvasObject> obj = std::make_shared<Obj>(CanvasObject::Coord{left, bottom});
    RenderingQueue::getInstance().emplace(obj, std::make_unique<View>());
    obj->setSize(right - left, top - bottom);
    return ObjectHandler<Obj, View, PostProcess>::postprocessObject(obj);
}


template <class PostProcess>
const CanvasObject* LineObjHandler<PostProcess>::region(float x1, float y1, float x2, float y2) const
{
    AreaCmdBase::frame_view->setProperty(nullptr);
    std::shared_ptr<CanvasObject> obj = std::make_shared<LineObject>(CanvasObject::Coord{x1, y1});
    RenderingQueue::getInstance().emplace(obj, std::make_unique<LineView>());
    obj->setSize(x2 - x1, y2 - y1);
    return ObjectHandler<LineObject, LineView, PostProcess>::postprocessObject(obj);
}

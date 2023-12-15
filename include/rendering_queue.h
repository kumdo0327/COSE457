#ifndef __RENDERING_QUEUE_H__
#define __RENDERING_QUEUE_H__

#include "canvas_view.h"
#include "object_view.h"
#include "object_container.h"


class RenderingQueue : public Singleton<RenderingQueue>, public RenderingProcessor
{
public:
    void emplace(std::shared_ptr<CanvasObject> obj, std::unique_ptr<ObjectView> view)
    {
        _q.emplace_back(obj, std::move(view));
    }

    void render(float zoom) override
    {
        auto& container = ObjectContainer::getInstance();

        for (auto iter = _q.begin(); iter != _q.end(); ) {

            auto obj = iter->first.lock().get();
            if (obj) {// Canvas object is available

                obj->isSelected() ? iter->second->select() : iter->second->deselect();
                auto rgb = obj->getRGB();
                iter->second->render(ObjectView::RGB{rgb.r, rgb.g, rgb.b}, ObjectView::Coord{obj->getX(), obj->getY()},
                                    ObjectView::Size{obj->getWidth(), obj->getHeight()}, container.normalizeZ(obj->getZ()), zoom);
                ++iter;
            }
            else {// Canvas object is disavailable

                auto temp = iter;
                ++iter;
                _q.erase(temp);
            }
        }
    }
    
private:
    std::list<std::pair<std::weak_ptr<CanvasObject>, std::unique_ptr<ObjectView>>> _q;
};
#endif

#include "observers.h"


int Observers::_init(FrameView* frame_view)
{
    auto canvas = frame_view->getCanvas();
    auto& ref = ObjectContainer::getInstance();


    _canvas_updater = new ObjectObserver([canvas](const CanvasObject*){ canvas->render(); });

    _property_updater = new ObjectObserver([frame_view, &ref](const CanvasObject* obj){
    
        if (frame_view->getProperty() && ref.getSelection() == obj) {

            auto rgb = obj->getRGB();
            frame_view->getProperty()->setData(PropertyView::RGB{rgb.r, rgb.g, rgb.b}, obj->getX(), obj->getY(), obj->getWidth(), obj->getHeight());
        }
    });

    
    _composite_selection_updater = new ObjectObserver([](const CanvasObject* obj){

        if (obj->isSelected())
            AbstractPropertyFactory::getInstance().setPropertyFactory(std::make_unique<NullFactory>());
    });

    _line_selection_updater = new ObjectObserver([](const CanvasObject* obj){

        if (obj->isSelected())
            AbstractPropertyFactory::getInstance().setPropertyFactory(std::make_unique<PropertyFactory<LineProperty>>());
    });

    _rec_selection_updater = new ObjectObserver([](const CanvasObject* obj){

        if (obj->isSelected())
            AbstractPropertyFactory::getInstance().setPropertyFactory(std::make_unique<PropertyFactory<RecProperty>>());
    });

    _elps_selection_updater = new ObjectObserver([](const CanvasObject* obj){

        if (obj->isSelected())
            AbstractPropertyFactory::getInstance().setPropertyFactory(std::make_unique<PropertyFactory<ElpsProperty>>());
    });

    return 0;
}

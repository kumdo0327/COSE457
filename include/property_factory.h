#ifndef __PROPERTY_FACTORY_H__
#define __PROPERTY_FACTORY_H__

#include "property_view.h"


class PropertyFactoryBase
{
public:
    virtual ~PropertyFactoryBase() = default;
    virtual PropertyView* operator()(wxWindow* parent) const = 0;
};


class AbstractPropertyFactory : public Singleton<AbstractPropertyFactory>
{
public:
    PropertyView* createProperty(wxWindow* parent) const
    {
        return _factory ? (*_factory)(parent) : nullptr;
    }
    void setPropertyFactory(std::unique_ptr<PropertyFactoryBase> factory)
    {
        _factory = std::move(factory);
    }
    
private:
    std::unique_ptr<PropertyFactoryBase> _factory;
};




template <class View>
class PropertyFactory : public PropertyFactoryBase
{ public:
    PropertyView* operator()(wxWindow* parent) const override { return new View(parent); }
};


class NullFactory : public PropertyFactoryBase
{ public:
    PropertyView* operator()(wxWindow* parent) const override { return nullptr; }
};
#endif

#ifndef __CANVAS_OBJECT_H__
#define __CANVAS_OBJECT_H__

#include <list>
#include <cmath>


class CanvasObject;
class ObjectObserverBase
{
public:
    virtual ~ObjectObserverBase() = default;
    virtual void update(const CanvasObject* obj) const = 0;
};




class CanvasObject
{
public:
    typedef struct { float r, g, b; } RGB;
    typedef struct { float x, y; } Coord;
    typedef struct { float width, height; } Size;
public:
    static constexpr Coord DEFAULT_COORD    = {0.0f, 0.0f};
    static constexpr RGB DEFAULT_RGB        = {0.5f, 0.5f, 0.75f};
    static constexpr Size DEFAULT_SIZE      = {40.0f, 30.0f};

public:
    CanvasObject(Coord coord) : _coord(coord), _rgb(DEFAULT_RGB), _size(DEFAULT_SIZE), _selection_state(false), _notification_state(true) {}
    virtual ~CanvasObject() { deselect(); }

    void setZ(size_t z)                                     { _z = z; notify(); }
    void decreaseZ(size_t z)                                { if (z < _z) _z--; }
    void increaseZ(size_t z)                                { if (z > _z) _z++; }

    void setRGB(RGB rgb);
    virtual void setCoord(Coord coord)                      { _coord = coord; notify(); }
    virtual void setCoord(float x, float y)                 { _coord.x = x; _coord.y = y; notify(); }
    virtual void setX(float x)                              { _coord.x = x; notify(); }
    virtual void setY(float y)                              { _coord.y = y; notify(); }
    virtual void setSize(Size size)                         { _size = size; notify(); }
    virtual void setSize(float width, float height)         { _size.width = width; _size.height = height; notify(); }
    virtual void setWidth(float width)                      { _size.width = width; notify(); }
    virtual void setHeight(float height)                    { _size.height = height; notify(); }
    virtual void move(float dx, float dy)                   { _coord.x += dx; _coord.y += dy; notify(); }

    size_t getZ() const                                     { return _z; }
    RGB getRGB() const                                      { return _rgb; }
    Coord getCoord() const                                  { return _coord; }
    float getX() const                                      { return _coord.x; }
    float getY() const                                      { return _coord.y; }
    Size getSize() const                                    { return _size; }
    float getWidth() const                                  { return _size.width; }
    float getHeight() const                                 { return _size.height; }

    virtual bool hasPoint(float x, float y) = 0;
    virtual bool inRegion(float left, float right, float bottom, float top) = 0;

    virtual void select()                                   { _selection_state = true; notify(); }
    virtual void deselect()                                 { _selection_state = false; notify(); }
    bool isSelected() const                                 { return _selection_state; }

    void addObserver(const ObjectObserverBase* observer)    { _observers.push_back(observer); }
    void notify() const                                     { if (_notification_state) for (auto ptr : _observers) ptr->update(this); }
    void silence()                                          { _notification_state = false; }
    void sound()                                            { _notification_state = true; }


protected:
    static constexpr float THRESHOLD = 0.5f;

private:
    Coord _coord;
    size_t _z = 0;
    RGB _rgb;
    Size _size;

    std::list<const ObjectObserverBase*> _observers;
    bool _selection_state;
    bool _notification_state;
};




// Destructor = CanvasObject* -> void
template <class Destructor>
class CompositeObject : public CanvasObject
{
public:
    CompositeObject(Coord coord, Destructor destructor) : CanvasObject(coord), _destructor(destructor) {}
    virtual ~CompositeObject() override;
    void addObject(CanvasObject* obj)                                           { _list.push_back(obj); }
    void addObject(const std::list<std::shared_ptr<CanvasObject>>& objs)        { for (auto& ptr : objs) _list.push_back(ptr.get()); }

    void setCoord(Coord coord) override                                         { move(coord.x - getX(), coord.y - getY()); }
    void setCoord(float x, float y) override                                    { move(x - getX(), x - getY()); }
    void setX(float x) override                                                 { move(x - getX(), 0.0f); }
    void setY(float y) override                                                 { move(0.0f, y - getY()); }
    void move(float dx, float dy) override;

    bool hasPoint(float x, float y) override                                    { for (auto item : _list) if (item->hasPoint(x, y)) return true; return false; }
    bool inRegion(float left, float right, float bottom, float top) override    { return false; }
    bool empty() const                                                          { return _list.empty(); }

    void select() override;
    void deselect() override;

private:
    std::list<CanvasObject*> _list;
    Destructor _destructor;
};




class PolygonObject : public CanvasObject
{
public:
    PolygonObject(Coord coord) : CanvasObject(coord) {}

    void setSize(Size size) override                    { setSize(size.width, size.height); }
    void setSize(float width, float height) override    { CanvasObject::setSize(width < THRESHOLD ? THRESHOLD : width, height < THRESHOLD ? THRESHOLD : height); }
    void setWidth(float width) override                 { setSize(width, getHeight()); }
    void setHeight(float height) override               { setSize(getWidth(), height); }

    bool inRegion(float left, float right, float bottom, float top) override;
};




class LineObject : public CanvasObject
{
public:
    static constexpr float DEFAULT_LINE_WIDTH = 0.5f;
    static constexpr float DEFAULT_BORDER = 1.0f;
public:
    LineObject(Coord coord) : CanvasObject(coord), _line_width(DEFAULT_LINE_WIDTH), _border(DEFAULT_BORDER) {}

    void setSize(Size size) override                    { setSize(size.width, size.height); }
    void setSize(float width, float height) override;
    void setWidth(float width) override                 { setSize(width, getHeight()); }
    void setHeight(float height) override               { setSize(getWidth(), height); }

    void setLineWidth(float line_width, float border)   { _line_width = line_width; _border = border; notify(); }
    float getLineWidth() const                          { return _line_width; }

    bool hasPoint(float x, float y) override;
    bool inRegion(float left, float right, float bottom, float top) override;

private:
    static constexpr float _fit(float lb, float ub, float x)    { return x <= lb ? lb : (x >= ub ? ub : x); }
    static constexpr float _min(float a, float b)               { return a < b ? a : b; }
    static constexpr float _max(float a, float b)               { return a < b ? b : a; }

private:
    float _line_width;
    float _border;
};




class RecObject : public PolygonObject
{
public:
    RecObject(Coord coord) : PolygonObject(coord)   {}
    bool hasPoint(float x, float y) override        { return (getX() <= x && x <= getX() + getWidth()) && (getY() <= y && y <= getY() + getHeight()); }
};




class ElpsObject : public PolygonObject
{
public:
    ElpsObject(Coord coord) : PolygonObject(coord)  {}
    bool hasPoint(float x, float y) override;
};




template <class Update>
// Update = CanvasObject* -> void
class ObjectObserver : public ObjectObserverBase
{
public:
    ObjectObserver(Update update) : _update(update)     {}
    void update(const CanvasObject* obj) const override { _update(obj); }

private:
    Update _update;
};

#include "canvas_object.hpp"
#endif

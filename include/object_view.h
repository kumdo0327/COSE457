#ifndef __OBJECT_VIEW_H__
#define __OBJECT_VIEW_H__

#include <memory>
#include <cmath>

#if defined WIN32 || defined WIN64
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#include <unistd.h>
#endif


class ObjectView;
class SelectionState
{
public:
    virtual ~SelectionState() = default;
    virtual void select(ObjectView* view) = 0;
    virtual void deselect(ObjectView* view) = 0;
    virtual void render(float x1, float y1, float x2, float y2, float zoom) = 0;
};


class SelectedState : public SelectionState
{
public:
    void select(ObjectView* view) override      {}
    void deselect(ObjectView* view) override;
    void render(float x1, float y1, float x2, float y2, float zoom) override;
};


class UnselectedState : public SelectionState
{
public:
    void select(ObjectView* view) override;
    void deselect(ObjectView* view) override    {}
    void render(float x1, float y1, float x2, float y2, float zoom) override {}
};




class ObjectView
{
public:
    typedef struct { float r, g, b; } RGB;
    typedef struct { float x, y; } Coord;
    typedef struct { float width, height; } Size;

public:
    ObjectView() : _selection(std::make_unique<UnselectedState>()) {}
    virtual ~ObjectView() = default;

    void setState(std::unique_ptr<SelectionState> state) { _selection = std::move(state); }
    void select()   { _selection->select(this); }
    void deselect() { _selection->deselect(this); }

    void render(RGB rgb, Coord coord, Size size, float z, float zoom);

protected:
    virtual void renderObject(float x, float y, float width, float height, float z, float zoom) = 0;

private:
    std::unique_ptr<SelectionState> _selection;
};




class LineView : public ObjectView
{ protected:
    void renderObject(float x, float y, float width, float height, float z, float zoom) override;
};


class RecView : public ObjectView
{ protected:
    void renderObject(float x, float y, float width, float height, float z, float zoom) override;
};


class ElpsView : public ObjectView
{ protected:
    void renderObject(float x, float y, float width, float height, float z, float zoom) override;
};
#endif

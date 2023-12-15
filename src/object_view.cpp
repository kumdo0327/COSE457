#include "object_view.h"


void SelectedState::deselect(ObjectView* view) 
{
    view->setState(std::make_unique<UnselectedState>());
}


void UnselectedState::select(ObjectView* view) 
{
    view->setState(std::make_unique<SelectedState>());
}


void SelectedState::render(float x1, float y1, float x2, float y2, float zoom)
{
    float width = 1.5f;
    float left = (x1 < x2 ? x1 : x2) * zoom;
    float right = (x1 < x2 ? x2 : x1) * zoom;
    float bottom = (y1 < y2 ? y1 : y2) * zoom;
    float top = (y1 < y2 ? y2 : y1) * zoom;

    glColor4f(0.2f, 0.2f, 0.2f, 1.0f);
    glLineWidth(width * 2);
    glBegin(GL_LINE_LOOP);
        glVertex3f(left - width, bottom - width, 1.0f);
        glVertex3f(right + width, bottom - width, 1.0f);
        glVertex3f(right + width, top + width, 1.0f);
        glVertex3f(left - width, top + width, 1.0f);
    glEnd();
}


void ObjectView::render(RGB rgb, Coord coord, Size size, float z, float zoom)
{
    glColor4f(rgb.r, rgb.g, rgb.b, 1.0f);
    renderObject(coord.x, coord.y, size.width, size.height, z, zoom);
    _selection->render(coord.x, coord.y, coord.x + size.width, coord.y + size.height, zoom);
}




void LineView::renderObject(float x, float y, float width, float height, float z, float zoom)
{
    float line_width = 1.0f;

    glLineWidth(line_width * zoom);
    glBegin(GL_LINES);
        glVertex3f(x * zoom, y * zoom, z);
        glVertex3f((x + width) * zoom, (y + height) * zoom, z);
    glEnd();
}


void RecView::renderObject(float x, float y, float width, float height, float z, float zoom)
{
    x *= zoom;
    y *= zoom;
    width *= zoom;
    height *= zoom;

    glBegin(GL_QUADS);
        glVertex3f(x, y, z);
        glVertex3f(x + width, y, z);
        glVertex3f(x + width, y + height, z);
        glVertex3f(x, y + height, z);
    glEnd();
}


void ElpsView::renderObject(float x, float y, float width, float height, float z, float zoom)
{
    float rx = width / 2.0f * zoom;
    float ry = height / 2.0f * zoom;
    x = x * zoom + rx;
    y = y * zoom + ry;

    glBegin(GL_POLYGON);
    float inc = (float)M_PI_2 / (rx > ry ? rx : ry);
    for (float angle = 0.0f; angle < 2.0f * (float)M_PI; angle += inc)
        glVertex3f(x + std::cos(angle) * rx, y + std::sin(angle) * ry, z);
    glEnd();
}

#ifndef __MOUSE_ACTION_OVERLAY_H__
#define __MOUSE_ACTION_OVERLAY_H__

#include "canvas_view.h"


class MouseActionOverlay : public RenderingProcessor
{
public:
    MouseActionOverlay(float x1, float y1) : _x1(x1), _y1(y1) {}
    void setEnd(float x2, float y2)         { _x2 = x2; _y2 = y2; }

    void render(float zoom) override
    {
        float x1 = zoom * _x1;
        float y1 = zoom * _y1;
        float x2 = zoom * _x2;
        float y2 = zoom * _y2;
        float width = 1.0f;

        glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
        glLineWidth(width);
        glBegin(GL_LINE_LOOP);
            glVertex3f(x1, y1, 1.0f);
            glVertex3f(x2, y1, 1.0f);
            glVertex3f(x2, y2, 1.0f);
            glVertex3f(x1, y2, 1.0f);
        glEnd();

        glColor4f(0.55f, 0.71f, 0.73f, 0.5f);
        glBegin(GL_QUADS);
            glVertex3f(x1, y1, 1.0f);
            glVertex3f(x2, y1, 1.0f);
            glVertex3f(x2, y2, 1.0f);
            glVertex3f(x1, y2, 1.0f);
        glEnd();
    }

private:
    float _x1;
    float _y1;
    float _x2;
    float _y2;
};
#endif

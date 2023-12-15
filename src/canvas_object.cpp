#include "canvas_object.h"


void CanvasObject::setRGB(RGB rgb)
{
    _rgb.r = rgb.r < 0.0f ? 0.0f : (rgb.r > 1.0f ? 1.0f : rgb.r);
    _rgb.g = rgb.g < 0.0f ? 0.0f : (rgb.g > 1.0f ? 1.0f : rgb.g);
    _rgb.b = rgb.b < 0.0f ? 0.0f : (rgb.b > 1.0f ? 1.0f : rgb.b);
    notify();
}


void LineObject::setSize(float width, float height)
{
    if (-THRESHOLD < width && width < THRESHOLD && -THRESHOLD < height && height < THRESHOLD) {

        if (CanvasObject::getWidth() == 0);
        else if (CanvasObject::getWidth() > 0)
            width = THRESHOLD;
        else
            width = -THRESHOLD;

        if (CanvasObject::getHeight() == 0);
        else if (CanvasObject::getHeight() > 0)
            height = THRESHOLD;
        else
            height = -THRESHOLD;

        if (width < THRESHOLD && height < THRESHOLD) {

            width = THRESHOLD;
            height = 0;
        }
    }

    CanvasObject::setSize(width, height);
}


bool LineObject::hasPoint(float x, float y)
{
    float x1 = getX();
    float y1 = getY();
    float x2 = x1 + getWidth();
    float y2 = y1 + getHeight();
    float r = _line_width / 2 + _border;

    float proj_x = x1;
    float proj_y = y1;

    if (x1 == x2) // Vertical line
        proj_y = _fit(_min(y1, y2), _max(y1, y2), y);

    else if (y1 == y2) // Horizontal line
        proj_x = _fit(_min(x1, x2), _max(x1, x2), x);
    
    else {// Diagonal line

        float slope = (y2 - y1) / (x2 - x1);
        float inv_slope = -1 / slope;
        float x_ = (y - y1 + x1 * slope - x * inv_slope) / (slope - inv_slope);
        float y_ = slope * x_ + y1 - x1 * slope;
        proj_x = _fit(_min(x1, x2), _max(x1, x2), x_);
        proj_y = _fit(_min(y1, y2), _max(y1, y2), y_);
    }

    // distance <= radius
    return (x - proj_x) * (x - proj_x) + (y - proj_y) * (y - proj_y) <= r * r;
}


bool LineObject::inRegion(float left, float right, float bottom, float top)
{
    float x1 = getX();
    float y1 = getY();
    float x2 = x1 + getWidth();
    float y2 = y1 + getHeight();

    if (x1 == x2) // Vertical line
        return left <= x1 && x1 <= right && bottom <= y2 && top >= y1;

    if (y1 == y2) // Horizontal line
        return bottom <= y1 && y1 <= top && left <= x2 && right >= x1;

    // Included
    if (left <= _min(x1, x2) && _max(x1, x2) <= right && bottom <= _min(y1, y2) && _max(y1, y2) <= top)
        return true;

    // Diagonal line
    float slope = (y2 - y1) / (x2 - x1);
    auto fx = [slope, x1, y1](float x){ return slope * (x - x1) + y1; };
    auto fy = [slope, x1, y1](float y){ return (y - y1) / slope + x1; };
    
    if (_min(x1, x2) <= left && left <= _max(x1, x2)) { // Left
        
        auto y = fx(left);
        if (bottom <= y && y <= top)
            return true;
    }

    if (_min(x1, x2) <= right && right <= _max(x1, x2)) { // Right
        
        auto y = fx(right);
        if (bottom <= y && y <= top)
            return true;
    }

    if (_min(y1, y2) <= bottom && bottom <= _max(y1, y2)) { // Bottom
        
        auto x = fy(bottom);
        if (left <= x && x <= right)
            return true;
    }

    if (_min(y1, y2) <= top && top <= _max(y1, y2)) { // Top
        
        auto x = fy(top);
        if (left <= x && x <= right)
            return true;
    }

    return false;
}


bool PolygonObject::inRegion(float left, float right, float bottom, float top)
{
    return ((left <= getX() + getWidth()) && (right >= getX()) && (bottom <= getY() + getHeight()) && (top >= getY()));
}


bool ElpsObject::hasPoint(float x, float y)
{
    float rx = getWidth() / 2;
    float ry = getHeight() / 2;
    x = x - getX() - rx;
    y = y - getY() - ry;
    return ((x*x) / (rx*rx) + (y*y) / (ry*ry)) <= 1;
}

#ifndef __CANVAS_VIEW_H__
#define __CANVAS_VIEW_H__

#include <list>
#include <memory>
#include <cmath>

#include <wx/glcanvas.h>
#if defined WIN32 || defined WIN64
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#include <unistd.h>
#endif


class RenderingProcessor
{
public:
    virtual void render(float zoom) = 0;
};


class CanvasView : public wxGLCanvas
{
public:
    CanvasView(wxWindow* parent) : wxGLCanvas(parent), _context(std::make_unique<wxGLContext>(this)) { render(); }

    void render()                                       { beginRendering(); for (auto item : _pipeline) item->render(getScaledZoom()); endRendering(); }
    
    float getZoom() const                               { return GetSize().GetWidth() / _VIRTUAL_WIDTH; }
    float getScaledZoom(float coef = 2.0f) const        { return GetSize().GetWidth() * coef / _VIRTUAL_WIDTH; }
    float getInvertedZoom() const                       { return _VIRTUAL_WIDTH / GetSize().GetWidth(); }

    void pushFrontToPipeline(RenderingProcessor* p)     { _pipeline.push_front(p); }
    void pushBackToPipeline(RenderingProcessor* p)      { _pipeline.push_back(p); }
    void eraseRenderingProcessor(RenderingProcessor* p) { auto iter = std::find(_pipeline.begin(), _pipeline.end(), p); if (iter != _pipeline.end()) _pipeline.erase(iter); }

private:
    void beginRendering();
    void endRendering()                                 { glFlush(); SwapBuffers(); }

private:
    static constexpr float _VIRTUAL_WIDTH = 160.0f;
    static constexpr float _VIRTUAL_HEIGHT = 120.0f;

private:
    std::unique_ptr<wxGLContext> _context;
    std::list<RenderingProcessor*> _pipeline;
};
#endif

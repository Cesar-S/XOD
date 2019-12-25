
#ifndef X_GRAPHICS_H
#define X_GRAPHICS_H

#include <Arduino.h>

#include "XRender.h"
#include "XVector2.h"
#include "XFill.h"

using xod::XColor;

class XGraphics {
protected:
    XGraphics* _parent;
public:
    XGraphics();

    virtual const XFill* getFill() const;
    virtual const XColor* getBgColor() const;
    virtual const XColor* getStrokeColor() const;

    bool renderScanlineRecursively(XRenderer* renderer, int16_t line, uint16_t* buffer);
    virtual bool renderScanline(XRenderer* renderer, int16_t line, uint16_t* buffer) { return false; }
    void renderScreen(XRenderer* renderer);

    int16_t drawPointAtLine(int16_t scanline, XVector2<int16_t> A, XVector2<int16_t> B);
};

#include "XGraphics.inl"

#endif // X_GRAPHICS_H

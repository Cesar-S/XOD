
#ifndef CANVAS_H
#define CANVAS_H

#include "XGraphics.h"

class Canvas : public XGraphics {
private:
    XColor _color = { 0, 0, 0 };
    bool _dirty;

public:
    Canvas(XColor color);

    void setParams(XColor color);
    const XColor* getBgColor() const;
    bool renderScanline(XRenderer* renderer, int16_t scanline, uint16_t* buffer);
};

#include "Canvas.inl"

#endif // CANVAS_H

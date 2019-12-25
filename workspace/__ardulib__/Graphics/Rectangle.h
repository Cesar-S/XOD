
#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "XGraphics.h"

class Rectangle : public XGraphics {
private:
    XVector2<int16_t> _A;
    int16_t _w = 0;
    int16_t _h = 0;
    XVector2<int16_t> _old_A;
    int16_t _old_w = _w;
    int16_t _old_h = _h;

public:
    Rectangle(XGraphics* parent, int16_t x, int16_t y, int16_t w, int16_t h);

    void setParams(int16_t x, int16_t y, int16_t w, int16_t h);
    bool renderScanline(XRenderer* renderer, int16_t scanline, uint16_t* buffer);
};

#include "Rectangle.inl"

#endif // RECTANGLE_H

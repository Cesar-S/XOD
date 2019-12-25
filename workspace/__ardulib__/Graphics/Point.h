
#ifndef POINT_H
#define POINT_H

#include "XGraphics.h"

class Point : public XGraphics {
private:
    XVector2<int16_t> _A;
    XVector2<int16_t> _old_A;

public:
    Point(XGraphics* parent, int16_t x, int16_t y);

    void setParams(int16_t x, int16_t y);
    bool renderScanline(XRenderer* renderer, int16_t scanline, uint16_t* buffer);
};

#include "Point.inl"

#endif // POINT_H

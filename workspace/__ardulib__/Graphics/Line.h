
#ifndef LINE_H
#define LINE_H

#include "XGraphics.h"

class Line : public XGraphics {
private:
    XVector2<int16_t> _A;
    XVector2<int16_t> _B;
    XVector2<int16_t> _old_A;
    XVector2<int16_t> _old_B;

public:
    Line(XGraphics* parent, int16_t x0, int16_t y0, int16_t x1, int16_t y1);

    void setParams(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
    bool renderScanline(XRenderer* renderer, int16_t scanline, uint16_t* buffer);
};

#include "Line.inl"

#endif // LINE_H

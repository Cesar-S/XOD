
#ifndef STROKE_H
#define STROKE_H

#include "XGraphics.h"

class Stroke : public XGraphics {
private:
    XColor _color = { 0, 0, 0 };

public:
    Stroke(XGraphics* parent, XColor color) {
        _parent = parent;
        _color = color;
    }

    void setParams(XColor color);
    const XColor* getStrokeColor() const;
};

const XColor* Stroke::getStrokeColor() const {
    return &_color;
}

void Stroke::setParams(XColor color) {
    _color = color;
}

#endif // STROKE_H

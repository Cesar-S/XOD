
#ifndef FILL_SOLID_H
#define FILL_SOLID_H

#include "XGraphics.h"

class FillSolid : public XGraphics {
private:
    XFill* _fill = nullptr;

public:
    FillSolid(XGraphics* parent, XColor color) {
        _fill = new XFillSolid(color);
        _parent = parent;
    }

    void setParams(XColor color);
    const XFill* getFill() const;
};

void FillSolid::setParams(XColor color) {
    _fill->setSolidColor(color);
}

const XFill* FillSolid::getFill() const {
    return _fill;
}

#endif // FILL_SOLID_H

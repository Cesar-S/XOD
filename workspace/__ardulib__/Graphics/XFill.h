
#ifndef XFill_H
#define XFill_H

using xod::XColor;

class XFill {
public:
    XFill() {}
    virtual XColor colorAtPoint(XVector2<int16_t> point) = 0;
    virtual void setSolidColor(XColor _color) {}
};

class XFillSolid : public XFill {
private:
    XColor _color;
public:
    XFillSolid(XColor color) {
        _color = color;
    }

    XColor colorAtPoint(XVector2<int16_t> point);
    void setSolidColor(XColor _color);
};

XColor XFillSolid::colorAtPoint(XVector2<int16_t> point) {
    return _color;
}

void XFillSolid::setSolidColor(XColor color) {
    _color = color;
}

#endif // XFill_H

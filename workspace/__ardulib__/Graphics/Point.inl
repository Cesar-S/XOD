
Point::Point(XGraphics* parent, int16_t x, int16_t y) {
    _parent = parent;
    _A = XVector2<int16_t>(x, y);
    _old_A = _A;
}

void Point::setParams(int16_t x, int16_t y) {
    _old_A = _A;
    _A = XVector2<int16_t>(x, y);
}

bool Point::renderScanline(XRenderer* renderer, int16_t scanline, uint16_t* buffer) {
    bool res = false;

    if (getStrokeColor() == nullptr)
        return res;

    if (scanline == _A.y) {

        uint16_t color = color888to565(getStrokeColor()->r, getStrokeColor()->g, getStrokeColor()->b);
        buffer[_A.x] = color;
        res = true;
    }

    if (scanline == _old_A.y) {
        res = true;
    }

    return res;
}

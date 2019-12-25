
Rectangle::Rectangle(XGraphics* parent, int16_t x, int16_t y, int16_t w, int16_t h) {
    _parent = parent;
    _A = XVector2<int16_t>(x, y);
    _w = w;
    _h = h;
    _old_A = _A;
    _old_w = w;
    _old_h = h;
}

void Rectangle::setParams(int16_t x, int16_t y, int16_t w, int16_t h) {
    _old_A = _A;
    _old_w = _w;
    _old_h = _h;
    _A = XVector2<int16_t>(x, y);
    _w = w;
    _h = h;
}

bool Rectangle::renderScanline(XRenderer* renderer, int16_t scanline, uint16_t* buffer) {
    bool res = false;

    if (scanline >= _old_A.y && scanline <= _old_A.y + _old_h - 1) 
        res = true;

    if (getFill() != nullptr) {

        if (scanline >= _A.y && scanline <= _A.y + _h - 1) {

            XColor fillColor = getFill()->colorAtPoint(XVector2<int16_t>(0,0));

            uint16_t _fillColor = color888to565(fillColor.r, fillColor.g, fillColor.b);

            for (int16_t x = _A.x; x < _A.x + _w; x++)
                buffer[x] = _fillColor;

            res = true;
        }
    }

    if (getStrokeColor() != nullptr) {

        if (scanline == _A.y || scanline == _A.y + _h - 1) {

            uint16_t strokeColor = color888to565(getStrokeColor()->r, getStrokeColor()->g, getStrokeColor()->b);

            for (int16_t x = _A.x; x < _A.x + _w; x++)
                buffer[x] = strokeColor;

            res = true;
        }

        if (scanline > _A.y && scanline < _A.y + _h - 1) {

            uint16_t strokeColor = color888to565(getStrokeColor()->r, getStrokeColor()->g, getStrokeColor()->b);

            buffer[_A.x] = strokeColor;
            buffer[_A.x + _w - 1] = strokeColor;
            res = true;
        }
    }

    return res;
}

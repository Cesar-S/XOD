
Line::Line(XGraphics* parent, int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
    _parent = parent;
    _A  = XVector2<int16_t>(x0, y0);
    _B  = XVector2<int16_t>(x1, y1);
    _old_A = _A;
    _old_B = _B;
}

void Line::setParams(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
    _old_B = _B;
    _old_A = _A;
    _A = XVector2<int16_t>(x0, y0);
    _B = XVector2<int16_t>(x1, y1);
}

bool Line::renderScanline(XRenderer* renderer, int16_t scanline, uint16_t* buffer) {
    bool res = false;

    if (getStrokeColor() == nullptr) 
        return res;

    if (_old_A.y == _old_B.y) {

        if (scanline == _old_A.y)
            res = true;

    } else {

        int16_t old_minY = min(_old_A.y, _old_B.y);
        int16_t old_maxY = max(_old_A.y, _old_B.y);

        if (scanline >= old_minY && scanline <= old_maxY)
            res = true;  
    }

    if (_A.y == _B.y) {
        if (scanline == _A.y) {

            uint16_t color = color888to565(getStrokeColor()->r, getStrokeColor()->g, getStrokeColor()->b);

            if (_A.x != _B.x) {
                int16_t mixX = min(_A.x, _B.x);
                int16_t maxX = max(_A.x, _B.x);;
  
                for (int16_t x = mixX; x < maxX; x++)
                    buffer[x] = color;

                res = true;

            } else {

                buffer[_A.x] = color;
                res = true;
            }
        }
        
    } else {

        int16_t minY = min(_A.y, _B.y);
        int16_t maxY = max(_A.y, _B.y);

        if (scanline >= minY && scanline <= maxY) {

            uint16_t color = color888to565(getStrokeColor()->r, getStrokeColor()->g, getStrokeColor()->b);

            int16_t p = drawPointAtLine(scanline, _A, _B);
            buffer[p] = color;
            res = true;
        }
    }

    return res;
}

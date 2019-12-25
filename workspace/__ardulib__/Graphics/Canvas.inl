
Canvas::Canvas(XColor color) {
    _color = color;
    _dirty = true;
}

void Canvas::setParams(XColor color) {
    _color = color;
    _dirty = true;
}

const XColor* Canvas::getBgColor() const {
    return &_color;
}

bool Canvas::renderScanline(XRenderer* renderer, int16_t scanline, uint16_t* buffer) {
    bool res = false;

    if (getBgColor() == nullptr)
        return res;

    uint16_t color = color888to565(getBgColor()->r, getBgColor()->g, getBgColor()->b);
    int16_t endY = renderer->getHeight() - 1;
    int16_t endX = renderer->getWidth();

    for (int16_t x = 0; x < endX; x++)
        buffer[x] = color;

    if (_dirty) {
        if (scanline == endY) {
            _dirty = false;
        }
        res = true;
    }

    return res;
}

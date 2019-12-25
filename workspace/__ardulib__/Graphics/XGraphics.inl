
void swapInt16(int16_t& a, int16_t& b) {
    int16_t t = a;
    a = b;
    b = t;
}

XGraphics::XGraphics() {
    _parent = nullptr;
}

const XFill* XGraphics::getFill() const {
    return _parent ? _parent->getFill() : nullptr;
}

const XColor* XGraphics::getBgColor() const {
    return _parent ? _parent->getBgColor() : nullptr;
}

const XColor* XGraphics::getStrokeColor() const {
    return _parent ? _parent->getStrokeColor() : nullptr;
}

void XGraphics::renderScreen(XRenderer* renderer) {
    uint8_t bufferSize = renderer->getWidth();

    for (int16_t line = 0; line < renderer->getHeight(); line++) {
        uint16_t buffer[bufferSize];
        if (renderScanlineRecursively(renderer, line, buffer)) {
            renderer->setScanlinePart(line, 0, renderer->getWidth() - 1, buffer);
        }
    }
}

bool XGraphics::renderScanlineRecursively(XRenderer* renderer, int16_t line, uint16_t* buffer) {
    bool res = false;

    if (_parent != nullptr) {
        if (_parent->renderScanlineRecursively(renderer, line, buffer)) {
            res = true;
        }
    }

    if (renderScanline(renderer, line, buffer))
        res = true;

    return res;
}

int16_t XGraphics::drawPointAtLine(int16_t scanline, XVector2<int16_t> A, XVector2<int16_t> B) {
    int16_t steep = abs(B.y - A.y) > abs(B.x - A.x);
    if (steep) {
        swapInt16(A.x, A.y);
        swapInt16(B.x, B.y);
    }
    if (A.x > B.x) {
        swapInt16(A.x, B.x);
        swapInt16(A.y, B.y);
    }
    int16_t dx, dy;
    dx = B.x - A.x;
    dy = abs(B.y - A.y);
    int16_t err = dx / 2;
    int16_t ystep;
    if (A.y < B.y) {
        ystep = 1;
    } else {
        ystep = -1;
    }
    for (; A.x <= B.x; A.x++) {
        if (steep) {
            if (scanline == A.x) {
                return A.y;
            }
        } else {
            if (scanline == A.y) {
                return A.x;
            }
        }
        err -= dy;
        if (err < 0) {
            A.y += ystep;
            err += dx;
        }
    }
}

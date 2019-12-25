
#ifndef X_RENDERER_H
#define X_RENDERER_H

inline uint16_t swapColor565(uint16_t color) {
    return (color << 11) | (color & 0x07E0) | (color >> 11);
}

inline uint16_t color888to565(uint8_t red, uint8_t green, uint8_t blue) {
    return ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | (blue >> 3);
}

class XRenderer {
public:
    XRenderer() {}

    virtual void setScanlinePart(int16_t y, int16_t x0, int16_t x1, uint16_t* lineBuffer) = 0;

    int16_t getWidth() const {
        return _width;
    }

    int16_t getHeight() const {
        return _height;
    }

protected:
    int16_t _width = 0;
    int16_t _height = 0;
};

#endif // X_RENDERER_H

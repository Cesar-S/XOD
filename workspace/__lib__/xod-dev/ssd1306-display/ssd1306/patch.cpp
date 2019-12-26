#pragma XOD error_raise enable

// clang-format off
{{#global}}
// clang-format on

#include <XRender.h>
#include <Wire.h>

#define SSD1306_DISPLAY_OFF 0xAE
#define SSD1306_SET_DISPLAY_CLOCK 0xD5
#define SSD1306_SET_MULTIPLEX_RATIO 0xA8
#define SSD1306_SET_DISPLAY_OFFSET 0xD3
#define SSD1306_SET_START_LINE 0x40
#define SSD1306_CHARGE_DCDC_PUMP 0x8D
#define SSD1306_ADDR_MODE 0x20
#define SSD1306_SET_REMAP_L_TO_R 0xA1
#define SSD1306_SET_REMAP_T_TO_D 0xC8
#define SSD1306_SET_COM_PINS 0xDA
#define SSD1306_SET_CONTRAST 0x81
#define SSD1306_SET_PRECHARGE_PERIOD 0xD9
#define SSD1306_SET_VCOM_DESELECT 0xDB
#define SSD1306_RAM_ON 0xA4
#define SSD1306_INVERT_OFF 0xA6
#define SSD1306_DISPLAY_ON 0xAF
#define SSD1306_ADDR_PAGE 0x22
#define SSD1306_ADDR_COLUMN 0x21

class SSD1306 : public XRender {
public:
    SSD1306(TwoWire* wire, uint8_t i2cAddress = 0x00, uint8_t displayWidth = 0, uint8_t displayHeight = 0) {
        _wire = wire ? wire : &Wire;
        _i2cAddress = i2cAddress;
        _width = displayWidth;
        _height = displayHeight;
        _buffer = new uint8_t[_width * _height / 8];
    }

    ~SSD1306() {
        if (_buffer) {
            free(_buffer);
            _buffer = NULL;
        }
    }

    void begin();
    void sendBuffer();
    void clearScreen();

    void setLinePart(int16_t y, int16_t x0, int16_t x1, uint16_t* lineBuffer);
    void setLinePart(int16_t y, int16_t x0, int16_t x1, XColor color) {};
    void setPixel(int16_t x, int16_t y, XColor color);

private:
    TwoWire* _wire;
    uint8_t _i2cAddress;

    uint8_t* _buffer;

    void _sendCommand(uint8_t command);
};

void SSD1306::begin() {
    _wire->begin();
    _sendCommand(SSD1306_DISPLAY_OFF);
    _sendCommand(SSD1306_SET_DISPLAY_CLOCK);
    _sendCommand(0x80);
    _sendCommand(SSD1306_SET_MULTIPLEX_RATIO);
    _sendCommand(0x3F);
    _sendCommand(SSD1306_SET_DISPLAY_OFFSET);
    _sendCommand(0x00);
    _sendCommand(SSD1306_SET_START_LINE | 0);
    _sendCommand(SSD1306_CHARGE_DCDC_PUMP);
    _sendCommand(0x14);
    _sendCommand(SSD1306_ADDR_MODE);
    _sendCommand(0x00);
    _sendCommand(SSD1306_SET_REMAP_L_TO_R);
    _sendCommand(SSD1306_SET_REMAP_T_TO_D);
    _sendCommand(SSD1306_SET_COM_PINS);
    _sendCommand(0x12);
    _sendCommand(SSD1306_SET_CONTRAST);
    _sendCommand(0xFF);
    _sendCommand(SSD1306_SET_PRECHARGE_PERIOD);
    _sendCommand(0xF1);
    _sendCommand(SSD1306_SET_VCOM_DESELECT);
    _sendCommand(0x40);
    _sendCommand(SSD1306_RAM_ON);
    _sendCommand(SSD1306_INVERT_OFF);
    _sendCommand(SSD1306_DISPLAY_ON);
}

void SSD1306::sendBuffer() {
    _sendCommand(SSD1306_ADDR_PAGE);
    _sendCommand(0);
    _sendCommand(_height / 8 - 1);
    _sendCommand(SSD1306_ADDR_COLUMN);
    _sendCommand(0);
    _sendCommand(_width - 1);
    for (uint16_t i = 0; i < _width * _height / 8; i++) {
        _wire->beginTransmission(_i2cAddress);
        _wire->write(0x40);
        for (uint8_t x = 0; x < 16; x++) {
            _wire->write(_buffer[i++]);
        }
        i--;
        _wire->endTransmission();
    }
}

void SSD1306::clearScreen() {
    memset(_buffer, 0, _width * _height / 8);
}

void SSD1306::_sendCommand(uint8_t command) {
    _wire->beginTransmission(_i2cAddress);
    _wire->write(0x80);
    _wire->write(command);
    _wire->endTransmission();
}

void SSD1306::setPixel(int16_t x, int16_t y, XColor color) {
    if ((x < 0) || (x >= _width) || (y < 0) || (y >= _height))
        return;

    bool _color = color888toBool(color.r, color.g, color.b);

    uint8_t p = y / 8;
    uint16_t numByte = (p * 128) + x;
    uint8_t numBit = y % 8;
    switch (_color) {
    case 1:
        _buffer[numByte] |= 1 << numBit;
        break;
    case 0:
        _buffer[numByte] &= ~(1 << numBit);
        break;
    }
}

void SSD1306::setLinePart(int16_t y, int16_t x0, int16_t x1, uint16_t* lineBuffer) {
    if ((y >= _height) || (y < 0))
        return;
    if ((x0 < 0) || (x1 < 0) || (x0 >= _width) || (x1 >= _width) || (x0 > x1) || x0 == x1)
        return;

    for (int16_t x = 0; x < _width; x++) {

        bool _color = lineBuffer[x] < 0x7FFF ? 0 : 1;

        uint8_t p = y / 8;
        uint16_t numByte = (p * 128) + x;
        uint8_t numBit = y % 8;
        switch (_color) {
        case 1:
            _buffer[numByte] |= 1 << numBit;
            break;
        case 0:
            _buffer[numByte] &= ~(1 << numBit);
            break;
        }
    }
}

// clang-format off
{{/global}}
// clang-format on

struct State {
    uint8_t mem[sizeof(SSD1306)];
};

using Type = SSD1306*;

// clang-format off
{{ GENERATED_CODE }}
// clang-format on

void evaluate(Context ctx) {
    if (!isSettingUp())
        return;

    auto state = getState(ctx);

    auto i2c = getValue<input_I2C>(ctx);
    uint8_t addr = getValue<input_ADDR>(ctx);
    uint8_t width = (uint8_t)getValue<input_W>(ctx);
    uint8_t height = (uint8_t)getValue<input_H>(ctx);

    if (addr > 127) {
        raiseError(ctx);
        return;
    }

    Type dev = new (state->mem) SSD1306(i2c, addr, width, height);
    dev->begin();
    dev->clearScreen();

    emitValue<output_DEV>(ctx, dev);
}

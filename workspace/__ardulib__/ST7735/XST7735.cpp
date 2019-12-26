
#include "XST7735.h"

ST7735::ST7735(uint8_t cs, uint8_t dc, uint8_t rst) {
    _cs = cs;
    _rs = dc;
    _rst = rst;
    _hwSPI = true;
    _sid = _sclk = 0;

    _width = ST7735_TFTWIDTH;
    _height = ST7735_TFTHEIGHT;
}

void ST7735::setPixel(int16_t x, int16_t y, XColor color) {
    if ((x < 0) || (x >= _width) || (y < 0) || (y >= _height))
        return;

    uint16_t _color = color888to565(color.r, color.g, color.b);

    if (_tabcolor == INITR_BLACKTAB)
        _color = swapColor565(_color);

    setAddrWindow(x, y, x + 1, y + 1);

#ifdef SPI_HAS_TRANSACTION
    if (_hwSPI)
        SPI.beginTransaction(spisettings);
#endif

#ifdef __ARDUINO_ARC__
    digitalWrite(_rs, HIGH);
#else
    *rsport |= rspinmask;
#endif
    *csport &= ~cspinmask;

    spiwrite(_color >> 8);
    spiwrite(_color);

    *csport |= cspinmask;

#ifdef SPI_HAS_TRANSACTION
    if (_hwSPI)
        SPI.endTransaction();
#endif
}

void ST7735::setLinePart(int16_t y, int16_t x0, int16_t x1, uint16_t* lineBuffer) {
    if ((y >= _height) || (y < 0))
        return;
    if ((x0 < 0) || (x1 < 0) || (x0 >= _width) || (x1 >= _width) || (x0 > x1) || x0 == x1)
        return;

    if (_tabcolor == INITR_BLACKTAB)
        for (int16_t x = x0; x <= x1; x++)
            lineBuffer[x] = swapColor565(lineBuffer[x]);

    setAddrWindow(x0, y, x1, y);

#ifdef SPI_HAS_TRANSACTION
    if (_hwSPI)
        SPI.beginTransaction(spisettings);
#endif

#ifdef __ARDUINO_ARC__
    digitalWrite(_rs, HIGH);
#else
    *rsport |= rspinmask;
#endif
    *csport &= ~cspinmask;

    for (int16_t x = x0; x <= x1; x++) {
        uint8_t hi = lineBuffer[x] >> 8;
        uint8_t lo = lineBuffer[x];
        spiwrite(hi);
        spiwrite(lo);
    }

    *csport |= cspinmask;
#ifdef SPI_HAS_TRANSACTION
    if (_hwSPI)
        SPI.endTransaction();
#endif
}

void ST7735::setLinePart(int16_t y, int16_t x0, int16_t x1, XColor color) {
    if ((y >= _height) || (y < 0))
        return;
    if ((x0 < 0) || (x1 < 0) || (x1 >= _width) || (x0 >= _width) || (x0 > x1) || x0 == x1)
        return;

    uint16_t _color = color888to565(color.r, color.g, color.b);

    if (_tabcolor == INITR_BLACKTAB)
        _color = swapColor565(_color);

    setAddrWindow(x0, y, x1, y);

#ifdef SPI_HAS_TRANSACTION
    if (_hwSPI)
        SPI.beginTransaction(spisettings);
#endif

#ifdef __ARDUINO_ARC__
    digitalWrite(_rs, HIGH);
#else
    *rsport |= rspinmask;
#endif
    *csport &= ~cspinmask;

    for (int16_t x = x0; x <= x1; x++) {
        uint8_t hi = _color >> 8;
        uint8_t lo = _color;
        spiwrite(hi);
        spiwrite(lo);
    }

    *csport |= cspinmask;
#ifdef SPI_HAS_TRANSACTION
    if (_hwSPI)
        SPI.endTransaction();
#endif
}

void ST7735::setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
    writecommand(ST7735_CASET);
    writedata(0x00);
    writedata(x0 + colstart);
    writedata(0x00);
    writedata(x1 + colstart);

    writecommand(ST7735_RASET);
    writedata(0x00);
    writedata(y0 + rowstart);
    writedata(0x00);
    writedata(y1 + rowstart);

    writecommand(ST7735_RAMWR);
}

void ST7735::initB(void) {
    commonInit(Bcmd);
}

void ST7735::initG(void) {
    commonInit(Gcmd);
}

void ST7735::initR(uint8_t options) {
    commonInit(Rcmd1);
    if (options == INITR_GREENTAB) {
        commandList(Rcmd2green);
        colstart = 2;
        rowstart = 1;
    } else {
        commandList(Rcmd2red);
    }
    commandList(Rcmd3);
    _tabcolor = options;
}

void ST7735::writecommand(uint8_t c) {
#ifdef SPI_HAS_TRANSACTION
    if (_hwSPI)
        SPI.beginTransaction(spisettings);
#endif

#ifdef __ARDUINO_ARC__
    digitalWrite(_rs, LOW);
#else
    *rsport &= ~rspinmask;
#endif
    *csport &= ~cspinmask;

    spiwrite(c);

    *csport |= cspinmask;
#ifdef SPI_HAS_TRANSACTION
    if (_hwSPI)
        SPI.endTransaction();
#endif
}

inline void ST7735::spiwrite(uint8_t c) {
    if (_hwSPI) {
        SPI.transfer(c);
    } else {
        for (uint8_t bit = 0x80; bit; bit >>= 1) {
            if (c & bit)
                *dataport |= datapinmask;
            else
                *dataport &= ~datapinmask;
            *clkport |= clkpinmask;
            *clkport &= ~clkpinmask;
        }
    }
}

void ST7735::writedata(uint8_t c) {
#ifdef SPI_HAS_TRANSACTION
    if (_hwSPI)
        SPI.beginTransaction(spisettings);
#endif

#ifdef __ARDUINO_ARC__
    digitalWrite(_rs, HIGH);
#else
    *rsport |= rspinmask;
#endif
    *csport &= ~cspinmask;

    spiwrite(c);

    *csport |= cspinmask;
#ifdef SPI_HAS_TRANSACTION
    if (_hwSPI)
        SPI.endTransaction();
#endif
}

void ST7735::commandList(const uint8_t* addr) {
    uint8_t numCommands, numArgs;
    uint16_t ms;

    numCommands = pgm_read_byte(addr++); // Number of commands to follow
    while (numCommands--) { // For each command...
        writecommand(pgm_read_byte(addr++)); // Read, issue command
        numArgs = pgm_read_byte(addr++); // Number of args to follow
        ms = numArgs & DELAY; // If hibit set, delay follows args
        numArgs &= ~DELAY; // Mask out delay bit
        while (numArgs--) { // For each argument...
            writedata(pgm_read_byte(addr++)); // Read, issue argument
        }

        if (ms) {
            ms = pgm_read_byte(addr++); // Read post-command delay time (ms)
            if (ms == 255)
                ms = 500; // If 255, delay for 500 ms
            delay(ms);
        }
    }
}

void ST7735::commonInit(const uint8_t* cmdList) {
    colstart = rowstart = 0; // May be overridden in init func

    pinMode(_rs, OUTPUT);
    pinMode(_cs, OUTPUT);
    csport = portOutputRegister(digitalPinToPort(_cs));
    cspinmask = digitalPinToBitMask(_cs);
    rsport = portOutputRegister(digitalPinToPort(_rs));
    rspinmask = digitalPinToBitMask(_rs);

    if (_hwSPI) { // Using hardware SPI
        SPI.begin();

#ifdef SPI_HAS_TRANSACTION
        spisettings = SPISettings(4000000L, MSBFIRST, SPI_MODE0);
#else
#if defined(ARDUINO_ARCH_SAM)
        SPI.setClockDivider(24); // 4 MHz (half speed)
#else
        SPI.setClockDivider(SPI_CLOCK_DIV4); // 4 MHz (half speed)
#endif
        SPI.setBitOrder(MSBFIRST);
        SPI.setDataMode(SPI_MODE0);
#endif // SPI_HAS_TRANSACTION

    } else {
        pinMode(_sclk, OUTPUT);
        pinMode(_sid, OUTPUT);
        clkport = portOutputRegister(digitalPinToPort(_sclk));
        clkpinmask = digitalPinToBitMask(_sclk);
        dataport = portOutputRegister(digitalPinToPort(_sid));
        datapinmask = digitalPinToBitMask(_sid);
        *clkport &= ~clkpinmask;
        *dataport &= ~datapinmask;
    }

    // toggle RST low to reset; CS low so it'll listen to us
    *csport &= ~cspinmask;
    if (_rst) {
        pinMode(_rst, OUTPUT);
        digitalWrite(_rst, HIGH);
        delay(500);
        digitalWrite(_rst, LOW);
        delay(500);
        digitalWrite(_rst, HIGH);
        delay(500);
    }

    if (cmdList)
        commandList(cmdList);
}

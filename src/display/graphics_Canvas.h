#pragma once

#include "pico.h"

namespace graphics {

class Canvas
{
public:
    Canvas(size_t width, size_t height);
    ~Canvas();

    size_t getWidth() {
        return _width;
    }
    size_t getHeight() {
        return _height;
    }

    /**
     * Draw a single pixel to the display
     */
    virtual void setPixel(size_t x, size_t y, uint16_t colour) = 0;
    virtual uint16_t getPixel(size_t x, size_t y) = 0;

    virtual void drawVLine(size_t x, size_t y, size_t length, uint16_t colour) = 0;
    virtual void drawHLine(size_t x, size_t y, size_t length, uint16_t colour) = 0;

    virtual void invert() = 0;
    virtual void fill(uint16_t colour) = 0;

protected:
    size_t _width;
    size_t _height;

};


/**
 * 1Bit Per Pixel Canvas
 */
class Canvas1Bit : public Canvas
{
public:
    Canvas1Bit(size_t width, size_t height);
    ~Canvas1Bit();

    virtual void setPixel(size_t x, size_t y, uint16_t colour);
    virtual uint16_t getPixel(size_t x, size_t y);
    virtual void drawVLine(size_t x, size_t y, size_t length, uint16_t colour);
    virtual void drawHLine(size_t x, size_t y, size_t length, uint16_t colour);
    virtual void invert();
    virtual void fill(uint16_t colour);

    size_t bufferLen();
    const uint8_t* getBuffer();

private:
    size_t _row_len;  // Length of an individual row in bytes
    size_t _buffer_len;  // Length of buffer in bytes
    uint8_t *_buffer;

};

}  // !graphics

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
    virtual void drawPixel(size_t x, size_t y, uint16_t colour) = 0;

    /**
     * Invert colours
     */
    virtual void invert() = 0;

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
    Canvas1Bit(size_t width, size_t height, uint8_t *bytes);
    ~Canvas1Bit();

    virtual void drawPixel(size_t x, size_t y, uint16_t colour);
    virtual void invert();

    size_t bufferLen();
    const uint8_t* getBuffer();

private:
    size_t _buffer_len;
    uint8_t *_buffer;

};

}  // !graphics

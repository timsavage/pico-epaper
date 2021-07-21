#include <malloc.h>
#include <cstring>

#include "graphics_Canvas.h"

using namespace graphics;

Canvas::Canvas(size_t width, size_t height)
: _width(width),
  _height(height)
{}

Canvas::~Canvas()= default;


Canvas1Bit::Canvas1Bit(size_t width, size_t height)
: Canvas(width, height),
  _buffer(nullptr),
  _buffer_len(((width + 7) / 8) * height)
{
    if ((_buffer = (uint8_t *)malloc(_buffer_len))) {
        memset(_buffer, 0, _buffer_len);
    }
}

Canvas1Bit::Canvas1Bit(size_t width, size_t height, uint8_t *bytes)
: Canvas(width, height),
  _buffer(bytes),
  _buffer_len(((width + 7) / 8) * height)
{
    if ((_buffer = (uint8_t *)malloc(_buffer_len))) {
        memset(_buffer, 0, _buffer_len);
    }
}

Canvas1Bit::~Canvas1Bit()
{
    if (_buffer) {
        free(_buffer);
    }
}

void Canvas1Bit::drawPixel(size_t x, size_t y, uint16_t colour)
{
    if (_buffer == nullptr || (x > _width) || (y > _height)) {
        return;
    }

    size_t block_idx = (x / 8) + (y * ((_width + 7) / 8));
    uint8_t *block = &_buffer[block_idx];
    if (colour) {
        *block |= 0x80 >> (x & 7);
    } else {
        *block &= ~(0x80 >> (x & 7));
    }
}

void Canvas1Bit::invert()
{
    for (size_t idx = 0; idx < _buffer_len; idx++) {
        _buffer[idx] = !_buffer[idx];
    }
}

size_t Canvas1Bit::bufferLen()
{
    return _buffer_len;
}

const uint8_t* Canvas1Bit::getBuffer()
{
    return _buffer;
}

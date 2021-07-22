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
  _row_len((width + 7) / 8),
  _buffer_len(_row_len * height),
  _buffer(nullptr)
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

void Canvas1Bit::setPixel(size_t x, size_t y, uint16_t colour)
{
    if (_buffer == nullptr || (x > _width) || (y > _height)) return;

    size_t block_idx = (x / 8) + (y * _row_len);
    uint8_t *block = &_buffer[block_idx];
    if (colour) {
        *block |= 0x80 >> (x & 7);
    } else {
        *block &= ~(0x80 >> (x & 7));
    }
}
uint16_t Canvas1Bit::getPixel(size_t x, size_t y)
{
    if (_buffer == nullptr || (x > _width) || (y > _height)) return 0;

    auto block_idx = (x / 8) + (y * ((_width + 7) / 8));
    return  (_buffer[block_idx] & (0x80 >> (x & 7))) != 0;
}

void Canvas1Bit::drawVLine(size_t x, size_t y, size_t length, uint16_t colour)
{
    if (_buffer == nullptr || (x > _width) || (y > _height)) return;

    // Limit length to height
    if (y + length > _height) {
        length = _height - y;
    }
    uint8_t *ptr = &_buffer[(x / 8) + (y * _row_len)];

    if (colour) {
        uint8_t block_mask = (0x80 >> (x & 7));
        for (size_t idx = 0; idx < length; idx++) {
            *ptr |= block_mask;
            ptr += _row_len;
        }
    } else {
        uint8_t block_mask = ~(0x80 >> (x & 7));
        for (size_t idx = 0; idx < length; idx++) {
            *ptr &= block_mask;
            ptr += _row_len;
        }
    }
}

void Canvas1Bit::drawHLine(size_t x, size_t y, size_t length, uint16_t colour)
{

}

void Canvas1Bit::invert()
{
    if (_buffer == nullptr) return;

    for (size_t idx = 0; idx < _buffer_len; idx++) {
        _buffer[idx] = !_buffer[idx];
    }
}

void Canvas1Bit::fill(uint16_t colour)
{
    if (_buffer == nullptr) return;
    memset(_buffer, colour ? 0xFF : 0x00, _buffer_len);
}

size_t Canvas1Bit::bufferLen()
{
    return _buffer_len;
}

const uint8_t* Canvas1Bit::getBuffer()
{
    return _buffer;
}

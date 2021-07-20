#pragma once

#include "pico/stdlib.h"

namespace graphics {

class Context
{
public:
    Context(size_t width, size_t height);
    ~Context();

private:
    size_t _width;
    size_t _height;
};

}  // !graphics

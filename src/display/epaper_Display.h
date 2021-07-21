#pragma once

#include "pico.h"

#include "epaper_Hal.h"
#include "graphics_Canvas.h"

#define EPD_WIDTH   152
#define EPD_HEIGHT  296

#define EPD_LAYER_BLACK = 0
#define EPD_LAYER_RED = 1

namespace display {
namespace epaper {

    class Display
    {
    public:
        /**
         * Constructor
         */
        explicit Display(Hal *hal);

        ~Display();

        /**
         * Initialise display
         */
        uint init();

        /**
         * Put display into deep-sleep mode, use init to awaken.
         */
        void sleep();

        /**
         * Flip display buffers
         */
        void flip();

        /**
         * Draw a canvas to the display
         */
        size_t drawCanvas(graphics::Canvas1Bit *canvas, uint8_t layer);
        size_t drawCanvas(graphics::Canvas1Bit *canvas, uint8_t layer, size_t x, size_t y);

        /**
         * Set frame
         */
        void setFrame(
            const uint8_t *black_buffer,
            const uint8_t *red_buffer,
            size_t buffer_width,
            size_t buffer_height
        );

        /**
         * Clear frame
         */
        void clearFrame();

        size_t width;
        size_t height;

    private:

        void setAddressWindow(size_t x_start, size_t y_start, size_t x_end, size_t y_end);

        void setAddress(size_t x, size_t y);

        Hal *_hal;
    };

} // !epaper
} // !display

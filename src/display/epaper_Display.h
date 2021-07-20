#pragma once

#include "pico/stdlib.h"

#include "epaper_Hal.h"

#define PIN_MISO  16
#define PIN_CS    17
#define PIN_SCK   18  // SCL
#define PIN_MOSI  19  // SDA
#define PIN_RESET 22
#define PIN_DC    20
#define PIN_BUSY  21

#define EPD_WIDTH       152
#define EPD_HEIGHT      296
#define EPD_BUFFER_SIZE ((EPD_WIDTH / 8) * EPD_HEIGHT)

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

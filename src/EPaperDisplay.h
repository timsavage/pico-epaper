#pragma once

#include "pico/stdlib.h"
#include "hardware/spi.h"

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

namespace Display {

    class EPaperDisplay {
    public:
        /**
         * Constructor
         * @param spi SPI port
         * @param pin_miso MISO Pin
         * @param pin_cs Chip Select Pin
         * @param pin_scl SPI Clock
         * @param pin_mosi MOSI Pin
         * @param pin_dc Data/Control pin
         * @param pin_reset Reset pin (used to reset the display)
         * @param pin_busy Indicates if the display is busy
         */
        EPaperDisplay(
            spi_inst_t * spi,
            uint pin_miso,
            uint pin_cs,
            uint pin_scl,
            uint pin_mosi,
            uint pin_dc,
            uint pin_reset,
            uint pin_busy
        );
        ~EPaperDisplay();

        /**
         * Hardware setup
         */
        uint setup();

        /**
         * Reset display
         */
        void reset() const;

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
            const uint8_t* black_buffer,
            const uint8_t* red_buffer,
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

        void command(uint8_t cmd);
        void command(uint8_t cmd, uint8_t d1);
        void command(uint8_t cmd, uint8_t d1, uint8_t d2);
        void command(uint8_t cmd, uint8_t d1, uint8_t d2, uint8_t d3);
        void command(uint8_t cmd, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4);
        void command_start(uint8_t cmd);
        void command_data(uint8_t data);
        void command_data(const uint8_t *data, size_t len);
        void command_end() const;

        void waitUntilIdle() const;
        void setAddressWindow(size_t x_start, size_t y_start, size_t x_end, size_t y_end);
        void setAddress(size_t x, size_t y);

        spi_inst_t *_spi;
        uint _pin_miso;
        uint _pin_cs;
        uint _pin_scl;
        uint _pin_mosi;
        uint _pin_dc;
        uint _pin_reset;
        uint _pin_busy;
    };
}

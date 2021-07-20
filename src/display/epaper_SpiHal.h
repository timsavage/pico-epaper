#pragma once

#include "pico/stdlib.h"
#include "hardware/spi.h"

#include "epaper_Hal.h"

namespace display {
namespace epaper {

    /**
     * SPI Interface for epaper::Display
     */
    class SpiHal : public Hal{
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
        SpiHal(
            spi_inst_t * spi,
            uint pin_miso,
            uint pin_cs,
            uint pin_scl,
            uint pin_mosi,
            uint pin_dc,
            uint pin_reset,
            uint pin_busy
        );
        ~SpiHal();

        /**
         * Hardware setup
         */
        virtual uint setup();

        /**
         * Reset display
         */
        virtual void reset() const;

        /**
         * Wait for the busy line to clear
         */
        virtual void waitUntilIdle() const;

        /**
         * Transaction control
         */
        virtual void startTransaction() const;
        virtual void endTransaction() const;

        /**
         * Command and data methods
         */
        virtual void command(uint8_t cmd);
        virtual void data(uint8_t d1);
        virtual void data(uint8_t d1, uint8_t d2);
        virtual void data(uint8_t d1, uint8_t d2, uint8_t d3);
        virtual void data(uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4);
        virtual void data_n(const uint8_t *data, size_t len);

    private:
        spi_inst_t *_spi;
        uint _pin_miso;
        uint _pin_cs;
        uint _pin_scl;
        uint _pin_mosi;
        uint _pin_dc;
        uint _pin_reset;
        uint _pin_busy;
    };

}  // !epaper
}  // !display
#pragma once

#include "pico/stdlib.h"

namespace display {
namespace epaper {

    class Hal
    {
    public:
        /**
         * Hardware setup
         */
        virtual uint setup() = 0;

        /**
         * Reset display
         */
        virtual void reset() const = 0;

        /**
         * Wait for the busy line to clear
         */
        virtual void waitUntilIdle() const = 0;

        /**
         * Start a transaction with the display
         */
        virtual void startTransaction() const = 0;

        /**
         * End transaction with display
         */
        virtual void endTransaction() const = 0;

        /**
         * Start a command
         * @param cmd Command to send
         */
        virtual void command(uint8_t cmd) = 0;

        /**
         * Send command data
         */
        virtual void data(uint8_t d1) = 0;
        virtual void data(uint8_t d1, uint8_t d2) = 0;
        virtual void data(uint8_t d1, uint8_t d2, uint8_t d3) = 0;
        virtual void data(uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4) = 0;

        /**
         * Send command data from a buffer
         */
        virtual void data_n(const uint8_t *data, size_t len) = 0;
    };

}  // !epaper
}  // !display
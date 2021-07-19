#pragma once

#include "pico/stdlib.h"
#include "hardware/spi.h"

namespace Display
{
    class EPaperHAL
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


    /**
     * SPI Interface for EPaperDisplay
     */
    class EPaperSPI : public EPaperHAL{
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
        EPaperSPI(
            spi_inst_t * spi,
            uint pin_miso,
            uint pin_cs,
            uint pin_scl,
            uint pin_mosi,
            uint pin_dc,
            uint pin_reset,
            uint pin_busy
        );
        ~EPaperSPI();

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

    /**
     * Basic transaction
     */
    class Transaction
    {
    public:
        Transaction(EPaperHAL *hal);
        ~Transaction();

    private:
        EPaperHAL *_hal;
    };

    /**
     * Synchronous transaction that waits for busy signal to be cleared on end.
     */
    class SyncTransaction
    {
    public:
        SyncTransaction(EPaperHAL *hal);
        ~SyncTransaction();

    private:
        EPaperHAL *_hal;
    };

}; // !Display

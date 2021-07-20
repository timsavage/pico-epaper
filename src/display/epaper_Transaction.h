#pragma once

#include "hardware/spi.h"

#include "epaper_Hal.h"

namespace display {
namespace epaper {

    /**
     * Basic transaction
     */
    class Transaction
    {
    public:
        explicit Transaction(Hal *hal);
        ~Transaction();

    private:
        Hal *_hal;
    };

    /**
     * Synchronous transaction that waits for busy signal to be cleared on end.
     */
    class SyncTransaction
    {
    public:
        explicit SyncTransaction(Hal *hal);
        ~SyncTransaction();

    private:
        Hal *_hal;
    };

}  // !epaper
}  // !display

#include "epaper_Transaction.h"

using namespace display::epaper;


Transaction::Transaction(Hal *hal)
{
    _hal = hal;
    _hal->startTransaction();
}

Transaction::~Transaction()
{
    _hal->endTransaction();
}

SyncTransaction::SyncTransaction(Hal *hal)
{
    _hal = hal;
    _hal->startTransaction();
}

SyncTransaction::~SyncTransaction()
{
    _hal->endTransaction();
    _hal->waitUntilIdle();
}

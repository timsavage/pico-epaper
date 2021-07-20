#include "epaper_SpiHal.h"

using namespace display::epaper;

SpiHal::SpiHal(
    spi_inst_t *spi,
    uint pin_miso,
    uint pin_cs,
    uint pin_scl,
    uint pin_mosi,
    uint pin_dc,
    uint pin_reset,
    uint pin_busy
)
: _spi(spi),
  _pin_miso(pin_miso),
  _pin_cs(pin_cs),
  _pin_scl(pin_scl),
  _pin_mosi(pin_mosi),
  _pin_dc(pin_dc),
  _pin_reset(pin_reset),
  _pin_busy(pin_busy)
{ }

SpiHal::~SpiHal() = default;

uint SpiHal::setup()
{
    // Configure SPI
    spi_init(_spi, 4000 * 1000); // 4Mbps
    gpio_set_function(_pin_miso, GPIO_FUNC_SPI);
    gpio_set_function(_pin_scl, GPIO_FUNC_SPI);
    gpio_set_function(_pin_mosi, GPIO_FUNC_SPI);

    // Configure CS and default high
    gpio_init(_pin_cs);
    gpio_set_dir(_pin_cs, GPIO_OUT);
    gpio_put(_pin_cs, true);

    // Configure DC and Reset pins
    gpio_init(_pin_dc);
    gpio_set_dir(_pin_dc, GPIO_OUT);
    gpio_init(_pin_reset);
    gpio_set_dir(_pin_reset, GPIO_OUT);

    // Configure Busy pin
    gpio_init(_pin_busy);
    gpio_set_dir(_pin_busy, GPIO_IN);

    return 0;
}

void SpiHal::reset() const
{
    gpio_put(_pin_reset, false);
    sleep_ms(200);
    gpio_put(_pin_reset, true);
    sleep_ms(200);
}


void SpiHal::startTransaction() const
{
    gpio_put(_pin_cs, false);
}

void SpiHal::command(uint8_t cmd)
{
    gpio_put(_pin_dc, false);
    spi_write_blocking(_spi, &cmd, 1);
    gpio_put(_pin_dc, true);
}

void SpiHal::data(uint8_t d1)
{
    spi_write_blocking(_spi, &d1, 1);
}

void SpiHal::data(uint8_t d1, uint8_t d2)
{
    spi_write_blocking(_spi, &d1, 1);
    spi_write_blocking(_spi, &d2, 1);
}

void SpiHal::data(uint8_t d1, uint8_t d2, uint8_t d3)
{
    spi_write_blocking(_spi, &d1, 1);
    spi_write_blocking(_spi, &d2, 1);
    spi_write_blocking(_spi, &d3, 1);
}

void SpiHal::data(uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4)
{
    spi_write_blocking(_spi, &d1, 1);
    spi_write_blocking(_spi, &d2, 1);
    spi_write_blocking(_spi, &d3, 1);
    spi_write_blocking(_spi, &d4, 1);
}

void SpiHal::data_n(const uint8_t *data, size_t len)
{
    spi_write_blocking(_spi, data, len);
}

void SpiHal::endTransaction() const
{
    gpio_put(_pin_cs, true);
}

void SpiHal::waitUntilIdle() const
{
    while (gpio_get(_pin_busy)) {
        sleep_ms(100);
    }
}

#include "EPaperDisplay.h"

#define DRIVER_OUTPUT_CONTROL                       0x01
#define GATE_DRIVING_CONTROL                        0x03
#define SOURCE_DRIVING_CONTROL                      0x04
#define BOOSTER_SOFT_START_CONTROL                  0x0C
#define GATE_SCAN_START_POSITION                    0x0F
#define DEEP_SLEEP_MODE                             0x10
#define DATA_ENTRY_MODE_SETTING                     0x11
#define SW_RESET                                    0x12
#define HV_READY_DETECTION                          0x14
#define VCI_DETECTION                               0x15
#define TEMP_SENSOR_CONTROL                         0x18
#define TEMP_SENSOR_CONTROL_WRITE                   0x1A
#define TEMP_SENSOR_CONTROL_READ                    0x1B
#define TEMP_SENSOR_CONTROL_WRITE_EXTERNAL          0x1C
#define MASTER_ACTIVATION                           0x20
#define DISPLAY_UPDATE_CONTROL_1                    0x21
#define DISPLAY_UPDATE_CONTROL_2                    0x22
#define WRITE_RAM_BW                                0x24
#define WRITE_RAM_RED                               0x26
#define READ_RAM                                    0x27
#define VCOM_SENSE                                  0x28
#define VCOM_SENSE_DURATION                         0x29
#define PROGRAM_VCOM_OTP                            0x2A
#define WRITE_VCOM_REGISTER                         0x2C
#define OTP_REGISTER_READ                           0x2D
#define USER_ID_READ                                0x2E
#define STATUS_BIT_READ                             0x2F
#define PROGRAM_WS_OTP                              0x30
#define LOAD_WS_OTP                                 0x31
#define WRITE_LUT_REGISTER                          0x32
#define CRC_CALCULATION                             0x34
#define CRC_STATUS_READ                             0x35
#define PROGRAM_OTP_SELECTION                       0x36
#define OTP_REGISTER_WRITE                          0x37
#define USER_ID_REGISTER_WRITE                      0x38
#define OTP_PROGRAM_MODE                            0x39
#define SET_DUMMY_LINE_PERIOD                       0x3A
#define SET_GATE_TIME                               0x3B
#define BORDER_WAVEFORM_CONTROL                     0x3C
#define READ_RAM_OPTION                             0x41
#define SET_RAM_X_ADDRESS_START_END_POSITION        0x44
#define SET_RAM_Y_ADDRESS_START_END_POSITION        0x45
#define SET_RAM_X_ADDRESS_COUNTER                   0x4E
#define SET_RAM_Y_ADDRESS_COUNTER                   0x4F
#define SET_ANALOG_BLOCK_CONTROL                    0x74
#define SET_DIGITAL_BLOCK_CONTROL                   0x7E
#define NOP                                         0x7F

using namespace Display;

EPaperDisplay::EPaperDisplay(
    spi_inst_t *spi,
    uint pin_miso,
    uint pin_cs,
    uint pin_scl,
    uint pin_mosi,
    uint pin_dc,
    uint pin_reset,
    uint pin_busy
)
: width(EPD_WIDTH),
  height(EPD_HEIGHT),
  _spi(spi),
  _pin_miso(pin_miso),
  _pin_cs(pin_cs),
  _pin_scl(pin_scl),
  _pin_mosi(pin_mosi),
  _pin_dc(pin_dc),
  _pin_reset(pin_reset),
  _pin_busy(pin_busy)
{ }

EPaperDisplay::~EPaperDisplay() = default;

uint EPaperDisplay::setup()
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

void EPaperDisplay::reset() const
{
    gpio_put(_pin_reset, false);
    sleep_ms(200);
    gpio_put(_pin_reset, true);
    sleep_ms(200);
}

void EPaperDisplay::command(uint8_t cmd)
{
    gpio_put(_pin_cs, false);
    gpio_put(_pin_dc, false);
    spi_write_blocking(_spi, &cmd, 1);
    gpio_put(_pin_cs, true);
}
void EPaperDisplay::command(uint8_t cmd, uint8_t d1)
{
    gpio_put(_pin_cs, false);
    gpio_put(_pin_dc, false);
    spi_write_blocking(_spi, &cmd, 1);
    gpio_put(_pin_dc, true);
    spi_write_blocking(_spi, &d1, 1);
    gpio_put(_pin_cs, true);
}
void EPaperDisplay::command(uint8_t cmd, uint8_t d1, uint8_t d2)
{
    gpio_put(_pin_cs, false);
    gpio_put(_pin_dc, false);
    spi_write_blocking(_spi, &cmd, 1);
    gpio_put(_pin_dc, true);
    spi_write_blocking(_spi, &d1, 1);
    spi_write_blocking(_spi, &d2, 1);
    gpio_put(_pin_cs, true);
}
void EPaperDisplay::command(uint8_t cmd, uint8_t d1, uint8_t d2, uint8_t d3)
{
    gpio_put(_pin_cs, false);
    gpio_put(_pin_dc, false);
    spi_write_blocking(_spi, &cmd, 1);
    gpio_put(_pin_dc, true);
    spi_write_blocking(_spi, &d1, 1);
    spi_write_blocking(_spi, &d2, 1);
    spi_write_blocking(_spi, &d3, 1);
    gpio_put(_pin_cs, true);
}
void EPaperDisplay::command(uint8_t cmd, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4)
{
    gpio_put(_pin_cs, false);
    gpio_put(_pin_dc, false);
    spi_write_blocking(_spi, &cmd, 1);
    gpio_put(_pin_dc, true);
    spi_write_blocking(_spi, &d1, 1);
    spi_write_blocking(_spi, &d2, 1);
    spi_write_blocking(_spi, &d3, 1);
    spi_write_blocking(_spi, &d4, 1);
    gpio_put(_pin_cs, true);
}

void EPaperDisplay::command_start(uint8_t cmd)
{
    gpio_put(_pin_cs, false);
    gpio_put(_pin_dc, false);
    spi_write_blocking(_spi, &cmd, 1);
    gpio_put(_pin_dc, true);
}

void EPaperDisplay::command_data(uint8_t data)
{
    spi_write_blocking(_spi, &data, 1);
}

void EPaperDisplay::command_data(const uint8_t *data, size_t len)
{
    spi_write_blocking(_spi, data, len);
}

void EPaperDisplay::command_end() const
{
    gpio_put(_pin_cs, true);
}

void EPaperDisplay::waitUntilIdle() const
{
    while (gpio_get(_pin_busy)) {
        sleep_ms(100);
    }
}

uint EPaperDisplay::init()
{
    if (setup() != 0) {
        return 1;
    }
    reset();

    command(
        DRIVER_OUTPUT_CONTROL,
        0x27,
        0x01,
        0x00
    );
    command(
        DATA_ENTRY_MODE_SETTING,
        0x03  // Pixel sequence Top Left -> Bottom right
    );
    command(
        SET_RAM_X_ADDRESS_START_END_POSITION,
        0x00,  // RAM x address start at 0
        0x12   // RAM x address end at 12h (18+1) * 8 -> 152
    );
    command(
        SET_RAM_Y_ADDRESS_START_END_POSITION,
        0x27,  // RAM y address start at C7h;
        0x01,
        0x00,  // RAM y address end at 00h;
        0x00
    );
    command(
        BORDER_WAVEFORM_CONTROL,
        0x01  // HIZ
    );
    command(
        DISPLAY_UPDATE_CONTROL_1,
        0x00,  // Normal
        0x80   // S8-S167
    );

    command(
        TEMP_SENSOR_CONTROL,
        0x80   // Internal temperature sensor
    );

    command(
        DISPLAY_UPDATE_CONTROL_2,
        0xB1  // Load Temperature and waveform setting.
    );

    command(MASTER_ACTIVATION);
    waitUntilIdle();

    return 0;
}

void EPaperDisplay::sleep()
{
    command(DEEP_SLEEP_MODE, 0x01);
    waitUntilIdle();
}

void EPaperDisplay::flip()
{
    command(DISPLAY_UPDATE_CONTROL_2,0xC7);
    command(MASTER_ACTIVATION);
    waitUntilIdle();
}

void EPaperDisplay::setFrame(
    const uint8_t* black_buffer, const uint8_t* red_buffer,
    size_t buffer_width, size_t buffer_height
)
{
    if (buffer_width > width
        || buffer_width % 8
        || buffer_height > height)
    {
        return;
    }
    size_t data_width = width / 8;

    setAddressWindow(0, 0, buffer_width - 1, buffer_height - 1);

    for (size_t y = 0; y < buffer_height; y++) {
        setAddress(0, y);
        command_start(WRITE_RAM_BW);
        command_data(&black_buffer[y * data_width], data_width);
        command_end();
    }

    for (size_t y = 0; y < buffer_height; y++) {
        setAddress(0, y);
        command_start(WRITE_RAM_RED);
        command_data(&red_buffer[y * data_width], data_width);
        command_end();
    }
}


void EPaperDisplay::clearFrame()
{
    setAddressWindow(0, 0, width - 1, height - 1);

    size_t buffer_width = width / 8;

    for (size_t y = 0; y < height; y++) {
        setAddress(0, y);
        command_start(WRITE_RAM_BW);
        for (size_t x = 0; x < buffer_width; x++) command_data(0xFF);
        command_end();
    }

    for (size_t y = 0; y < height; y++) {
        setAddress(0, y);
        command_start(WRITE_RAM_RED);
        for (size_t x = 0; x < buffer_width; x++) command_data(0x00);
        command_end();
    }
}

void EPaperDisplay::setAddressWindow(size_t x_start, size_t y_start, size_t x_end, size_t y_end)
{
    command(
        SET_RAM_X_ADDRESS_START_END_POSITION,
        (x_start >> 3) & 0xFF,
        (x_end >> 3) & 0xFF
    );
    command(
        SET_RAM_Y_ADDRESS_START_END_POSITION,
        y_start & 0xFF,
        (y_start >> 8) & 0xFF,
        y_end & 0xFF,
        (y_end >> 8) & 0xFF
    );
}

void EPaperDisplay::setAddress(size_t x, size_t y)
{
    command(
        SET_RAM_X_ADDRESS_COUNTER,
        (x >> 3) & 0xFF
    );
    command(
        SET_RAM_Y_ADDRESS_COUNTER,
        y & 0xFF,
        (y >> 8) & 0xFF
    );
}

#include "epaper_Display.h"
#include "epaper_Transaction.h"

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

using namespace display::epaper;

Display::Display(Hal *hal)
: width(EPD_WIDTH),
  height(EPD_HEIGHT),
  _hal(hal)
{ }

Display::~Display() = default;

uint Display::init()
{
    _hal->reset();

    SyncTransaction transaction(_hal);

    _hal->command(DRIVER_OUTPUT_CONTROL);
    _hal->data(0x27, 0x01, 0x00);

    _hal->command(DATA_ENTRY_MODE_SETTING);
    _hal->data(
        0x03  // Pixel sequence Top Left -> Bottom right
    );

    _hal->command(SET_RAM_X_ADDRESS_START_END_POSITION);
    _hal->data(
        0x00,  // RAM x address start at 0
        0x12   // RAM x address end at 12h (18+1) * 8 -> 152
    );

    _hal->command(SET_RAM_Y_ADDRESS_START_END_POSITION);
    _hal->data(
        0x27,  // RAM y address start at C7h;
        0x01,
        0x00,  // RAM y address end at 00h;
        0x00
    );

    _hal->command(BORDER_WAVEFORM_CONTROL);
    _hal->data(
        0x01  // HIZ
    );

    _hal->command(DISPLAY_UPDATE_CONTROL_1);
    _hal->data(
        0x00,  // Normal
        0x80   // S8-S167
    );

    _hal->command(TEMP_SENSOR_CONTROL);
    _hal->data(
        0x80   // Internal temperature sensor
    );

    _hal->command(DISPLAY_UPDATE_CONTROL_2);
    _hal->data(
        0xB1  // Load Temperature and waveform setting.
    );

    _hal->command(MASTER_ACTIVATION);

    return 0;
}

void Display::sleep()
{
    SyncTransaction transaction(_hal);

    _hal->command(DEEP_SLEEP_MODE);
    _hal->data(0x01);
}

void Display::flip()
{
    SyncTransaction transaction(_hal);

    _hal->command(DISPLAY_UPDATE_CONTROL_2);
    _hal->data(0xC7);

    _hal->command(MASTER_ACTIVATION);
}

void Display::setFrame(
    const uint8_t* black_buffer, const uint8_t* red_buffer,
    size_t buffer_width, size_t buffer_height
)
{
    Transaction transaction(_hal);

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
        _hal->command(WRITE_RAM_BW);
        _hal->data_n(&black_buffer[y * data_width], data_width);
    }

    for (size_t y = 0; y < buffer_height; y++) {
        setAddress(0, y);
        _hal->command(WRITE_RAM_RED);
        _hal->data_n(&red_buffer[y * data_width], data_width);
    }
}


void Display::clearFrame()
{
    Transaction transaction(_hal);
    size_t buffer_width = width / 8;

    setAddressWindow(0, 0, width - 1, height - 1);

    for (size_t y = 0; y < height; y++) {
        setAddress(0, y);

        _hal->command(WRITE_RAM_BW);
        for (size_t x = 0; x < buffer_width; x++) {
            _hal->data(0xFF);
        }
    }

    for (size_t y = 0; y < height; y++) {
        setAddress(0, y);

        _hal->command(WRITE_RAM_RED);
        for (size_t x = 0; x < buffer_width; x++) {
            _hal->data(0x00);
        }
    }
}

void Display::setAddressWindow(size_t x_start, size_t y_start, size_t x_end, size_t y_end)
{
    _hal->command(SET_RAM_X_ADDRESS_START_END_POSITION);
    _hal->data(
        (x_start >> 3) & 0xFF,
        (x_end >> 3) & 0xFF
    );

    _hal->command(SET_RAM_Y_ADDRESS_START_END_POSITION);
    _hal->data(
        y_start & 0xFF,
        (y_start >> 8) & 0xFF,
        y_end & 0xFF,
        (y_end >> 8) & 0xFF
    );
}

void Display::setAddress(size_t x, size_t y)
{
    _hal->command(SET_RAM_X_ADDRESS_COUNTER);
    _hal->data(
        (x >> 3) & 0xFF
    );

    _hal->command(SET_RAM_Y_ADDRESS_COUNTER);
    _hal->data(
        y & 0xFF,
        (y >> 8) & 0xFF
    );
}

#include <cstdio>
#include "pico/stdlib.h"
#include "hardware/spi.h"

#include "display/epaper_SpiHal.h"
#include "display/epaper_Display.h"
#include "display/graphics_Canvas.h"
#include "images.h"

const uint LED_PIN = 25;

#define PIN_MISO  16
#define PIN_CS    17
#define PIN_SCK   18  // SCL
#define PIN_MOSI  19  // SDA
#define PIN_RESET 22
#define PIN_DC    20
#define PIN_BUSY  21

display::epaper::SpiHal spiHal(spi0, PIN_MISO, PIN_CS, PIN_SCK, PIN_MOSI, PIN_DC, PIN_RESET, PIN_BUSY);
display::epaper::Display epaper(&spiHal);

graphics::Canvas1Bit canvas(epaper.width, epaper.height);

static inline void toggleLed()
{
    gpio_put(LED_PIN, !gpio_get(LED_PIN));
}

void setup()
{
    setup_default_uart();

    printf("Setup LED\n");
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    printf("Setup Display\n");
    spiHal.setup();
    epaper.init();
}

int main() {
    setup();

    printf("Set Frame\n");
    epaper.clearFrame();
    epaper.setFrame(SGV_BLACK, SGV_RED, epaper.width, 152);

    canvas.setPixel(10, 0, 1);
    canvas.setPixel(11, 0, 1);
    canvas.setPixel(12, 0, 1);
    canvas.setPixel(13, 0, 1);
    canvas.setPixel(14, 0, 1);
    canvas.setPixel(15, 0, 1);
    canvas.setPixel(16, 0, 1);
    canvas.drawVLine(13, 1, 8, 1);

    epaper.drawCanvas(&canvas, 1, 0, 160);

    printf("Flip\n");
    epaper.flip();

    printf("Sleep\n");
    epaper.sleep();

    while (true) {
        toggleLed();
        sleep_ms(500);
        toggleLed();
        sleep_ms(500);
    }

    return 0;
}

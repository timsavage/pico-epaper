#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

#include "EPaperDisplay.h"
#include "images.h"

const uint LED_PIN = 25;

#define PIN_MISO  16
#define PIN_CS    17
#define PIN_SCK   18  // SCL
#define PIN_MOSI  19  // SDA
#define PIN_RESET 22
#define PIN_DC    20
#define PIN_BUSY  21

Display::EPaperDisplay display(
    spi0, PIN_MISO, PIN_CS, PIN_SCK, PIN_MOSI, PIN_DC, PIN_RESET, PIN_BUSY
);

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
    display.init();
}

int main() {
    setup();

    printf("Set Frame\n");
    display.clearFrame();
    display.setFrame(SGV_BLACK, SGV_RED, display.width, 152);

    printf("Flip\n");
    display.flip();

    printf("Sleep\n");
    display.sleep();

    while (true) {
        toggleLed();
        sleep_ms(500);
        toggleLed();
        sleep_ms(500);
    }

    return 0;
}

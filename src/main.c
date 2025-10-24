#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/adc.h"

#define CONVERSION_FACTOR (3.3f / (1 << 12))
#define POLLING_RATE 50

const void print_banner(void);
const bool init_hardware(void);
const float read_adc_voltage(const uint8_t input);
const float read_temperature(void);
const void print_readings(const int count);
const void set_led(const bool state);

const int main(void) {
    stdio_init_all();

    if(!init_hardware())
        return -1;

    // allow for serial to connect before reading
    sleep_ms(2000);
    print_banner();

    int count = 0;

    while(true) {
        set_led(true);
        print_readings(count);

        sleep_ms(POLLING_RATE / 2);
        set_led(false);
        sleep_ms(POLLING_RATE / 2);

        count++;
    }

    return 0;
}

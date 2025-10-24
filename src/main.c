#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/adc.h"

#define CONVERSION_FACTOR (3.3f / (1 << 12))

void print_banner(void);
bool init_hardware(void);
float read_adc_voltage(uint8_t input);
float read_temperature(void);
void print_readings(int count);
void set_led(bool state);

int main(void) {
    stdio_init_all();

    if(!init_hardware())
        return -1;

    sleep_ms(2000);
    print_banner();

    int count = 0;

    while(true) {
        set_led(true);
        print_readings(count);
        sleep_ms(500);
        set_led(false);
        sleep_ms(500);

        count++;
    }
    return 0;
}

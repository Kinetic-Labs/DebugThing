#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/adc.h"

#define CONVERSION_FACTOR (3.3f / (1 << 12))

const void print_banner(void) {
    printf("\n");
    printf("                     ,----, \n");
    printf("                   ,/   .`| \n");
    printf("    ,---,        ,`   .'  : \n");
    printf("  .'  .' `\\    ;    ;     / \n");
    printf(",---.'     \\ .'___,/    ,'  \n");
    printf("|   |  .`\\  ||    :     |   \n");
    printf(":   : |  '  |;    |.';  ;   \n");
    printf("|   ' '  ;  :`----'  |  |   \n");
    printf("'   | ;  .  |    '   :  ;   \n");
    printf("|   | :  |  '    |   |  '   \n");
    printf("'   : | /  ;     '   :  |   \n");
    printf("|   | '` ,/      ;   |.'    \n");
    printf(";   :  .'        '---'      \n");
    printf("|   ,.'                     \n");
    printf("'---'                       \n");
    printf("\n");
}

const bool init_hardware(void) {
    if(cyw43_arch_init()) {
        printf("Failed to initialize cyw43_arch\n");
        return false;
    }

    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_gpio_init(26);
    adc_gpio_init(27);
    adc_gpio_init(28);

    return true;
}

const float read_adc_voltage(const uint8_t input) {
    adc_select_input(input);

    uint16_t raw = adc_read();

    return raw * CONVERSION_FACTOR;
}

const float read_temperature(void) {
    adc_select_input(4);

    const uint16_t raw = adc_read();
    const float voltage = raw * CONVERSION_FACTOR;

    return 27.0f - (voltage - 0.706f) / 0.001721f;
}

const void print_readings(const int count) {
    const float temp = read_temperature();

    printf("\n");
    printf("++++++++++++++++++++++++++++++++++++++\n");
    printf("++ Analog Input Reading (%d)\n", count);
    printf("[CPU Temperature: %.1f°C]\n", temp);

    const float v0 = read_adc_voltage(0);
    printf("  PROB 1: %.3fV\n", v0);

    const float v1 = read_adc_voltage(1);
    printf("  PROB 2: %.3fV\n", v1);

    const float v2 = read_adc_voltage(2);
    printf("  PROB 3: %.3fV\n", v2);
}

const void set_led(bool state) {
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, state ? 1 : 0);
}

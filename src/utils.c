#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/adc.h"

#define CONVERSION_FACTOR (3.3f / (1 << 12))

void print_banner(void) {
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

bool init_hardware(void) {
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

float read_adc_voltage(uint8_t input) {
    adc_select_input(input);
    uint16_t raw = adc_read();
    return raw * CONVERSION_FACTOR;
}

float read_temperature(void) {
    adc_select_input(4);

    uint16_t raw = adc_read();
    float voltage = raw * CONVERSION_FACTOR;

    return 27.0f - (voltage - 0.706f) / 0.001721f;
}

void print_readings(int count) {
    printf("\n--- Reading %d ---\n", count);

    float v0 = read_adc_voltage(0);
    printf("  PROB 1 (ADC0): %.3fV\n", v0);

    float v1 = read_adc_voltage(1);
    printf("  PROB 2 (ADC1): %.3fV\n", v1);

    float v2 = read_adc_voltage(2);
    printf("  PROB 3 (ADC2): %.3fV\n", v2);

    float temp = read_temperature();
    printf("  CPU Temperature: %.1f°C\n", temp);

    printf("----------------------------------------\n");
}

void set_led(bool state) {
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, state ? 1 : 0);
}

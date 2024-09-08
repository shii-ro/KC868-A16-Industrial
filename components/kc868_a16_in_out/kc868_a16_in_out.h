#include <stdint.h>
#include "esp_err.h"

#define PCF8574_BASE_ADDRESS (0b0100000)
#define PCF8574_INPUT_1   (PCF8574_BASE_ADDRESS | 0b010)
#define PCF8574_INPUT_0   (PCF8574_BASE_ADDRESS | 0b001)
#define PCF8574_OUTPUT_0  (PCF8574_BASE_ADDRESS | 0b100)
#define PCF8574_OUTPUT_1  (PCF8574_BASE_ADDRESS | 0b101)

esp_err_t kc868_a16_input_output_init();
uint16_t kc868_a16_inputs_read();
uint8_t kc868_a16_input_read(uint8_t index);
void kc868_a16_output_write(uint8_t index, uint8_t state);
void kc868_a16_outputs_write(uint16_t output_state);
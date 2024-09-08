#include "kc868_a16_in_out.h"
#include "pcf8574.h"

esp_err_t kc868_a16_input_output_init()
{
    esp_err_t err = 0;
    err = pcf8574_init();
    return err;
}

uint16_t kc868_a16_inputs_read()
{
    uint8_t input_0_state = 0;
    uint8_t input_1_state = 0;

    pcf8574_read_inputs(PCF8574_INPUT_0, &input_0_state);
    pcf8574_read_inputs(PCF8574_INPUT_1, &input_1_state);

    uint16_t input_state = (input_1_state << 8 | input_0_state);

    return input_state;
}

uint8_t kc868_a16_input_read(uint8_t index)
{
    if(index > 15)
    {
        return 0;
    }

    uint8_t input_state = 0;
    uint8_t index_state = 0;

    pcf8574_read_inputs(PCF8574_BASE_ADDRESS | (1 << (index > 7)), &input_state);
    index_state = (input_state >> index) & 0x1;

    return index_state;
}

void kc868_a16_output_write(uint8_t index, uint8_t state)
{
    if(index > 15)
    {
        return;
    }

    uint16_t output_state = (state << index);

    pcf8574_write_outputs(PCF8574_BASE_ADDRESS | (1 << (index > 7)), output_state);
}

void kc868_a16_outputs_write(uint16_t output_state)
{
    pcf8574_write_outputs(PCF8574_OUTPUT_0, (output_state >> 8));
    pcf8574_write_outputs(PCF8574_OUTPUT_1, (output_state & 0xFF));
}
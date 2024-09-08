#include "driver/i2c.h"
#include "driver/i2c_master.h"
#include <stdint.h>

#define I2C_MASTER_SCL_IO GPIO_NUM_5     // Define the SCL GPIO pin
#define I2C_MASTER_SDA_IO GPIO_NUM_4     // Define the SDA GPIO pin
#define I2C_MASTER_FREQ_HZ 100000   // Define I2C frequency (100kHz)
#define I2C_MASTER_NUM I2C_NUM_0 // I2C port number

esp_err_t pcf8574_init();
esp_err_t pcf8574_write_outputs(uint8_t i2c_slave_id, uint8_t out_state);
esp_err_t pcf8574_read_inputs(uint8_t i2c_slave_id, uint8_t* input_state);
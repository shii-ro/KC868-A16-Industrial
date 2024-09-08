#include "esp_log.h"
#include "pcf8574.h"

esp_err_t pcf8574_init()
{
    i2c_config_t i2c_config = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    esp_err_t err = i2c_param_config(I2C_MASTER_NUM, &i2c_config);
    if (err != ESP_OK) {
        return err;
    }

    return i2c_driver_install(I2C_MASTER_NUM, I2C_MODE_MASTER, 0, 0, 0);
}

esp_err_t pcf8574_write_outputs(uint8_t i2c_slave_id, uint8_t out_state)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (i2c_slave_id << 1 | I2C_MASTER_WRITE), true);
    i2c_master_write_byte(cmd, out_state, true);
    i2c_master_stop(cmd);
    
    esp_err_t err = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    
    if (err != ESP_OK) {
        ESP_LOGE("I2C", "I2C master failed: %s", esp_err_to_name(err));
    }
    
    return err;
}

esp_err_t pcf8574_read_inputs(uint8_t i2c_slave_id, uint8_t* input_state)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (i2c_slave_id << 1 | I2C_MASTER_READ), true);
    i2c_master_read(cmd, input_state, 1, true);
    i2c_master_stop(cmd);

    esp_err_t err = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    
    if (err != ESP_OK) {
        ESP_LOGE("I2C", "I2C master failed: %s", esp_err_to_name(err));
    }
    
    return err;
}
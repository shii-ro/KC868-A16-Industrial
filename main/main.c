#include <stdio.h>
#include "kc868_a16.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void)
{
    const char* SSID = "Rosangela 2.4";
    const char* PASSWORD = "filhotes23";
    const uint32_t PORT = 8001;

    if(kc868_a16_input_output_init() != ESP_OK)
    {
        return;
    };

    if(kc868_a16_wifi_init() == ESP_OK)
    {
        kc868_a16_wifi_connect(SSID, PASSWORD);
        kc868_a16_tcp_server_start_task(PORT);
    }

    while(true)
    {
        uint16_t inputs_state = 0;
        inputs_state = kc868_a16_inputs_read();
        kc868_a16_tcp_server_send_message(inputs_state);

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
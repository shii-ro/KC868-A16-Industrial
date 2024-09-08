idf_component_register(
    SRCS "kc868_a16_wifi.c"
    INCLUDE_DIRS "."
    REQUIRES esp_wifi nvs_flash
)

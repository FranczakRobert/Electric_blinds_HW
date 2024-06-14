idf_component_register(SRCS "server.c" "wifi.c" "main.c" "led.c" "server.c"
                    INCLUDE_DIRS "." "includes")
spiffs_create_partition_image(storage spiffs_image FLASH_IN_PROJECT)

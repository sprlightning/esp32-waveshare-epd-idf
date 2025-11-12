# -- CMAKE for Espressif IDF
# -- author MLX
# -- copyright GPLv3

idf_component_register(
    SRC_DIRS src src/utility examples/epd2in13_V4-demo
    INCLUDE_DIRS src src/utility examples/epd2in13_V4-demo
    REQUIRES esp_common freertos hal log nvs_flash driver esp_timer
)

target_compile_options(${COMPONENT_LIB} PUBLIC -DESP32=1 -DESP32_CMAKE=1 -Wno-error -Wno-format -fpermissive)

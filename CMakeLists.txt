# -- CMAKE for Espressif IDF
# -- author MLX
# -- copyright GPLv3

set(EPD_DEMO_PATH "examples/epd2in7_V2-demo")

idf_component_register(
    SRC_DIRS src src/utility src/GUI src/Fonts ${EPD_DEMO_PATH}
    INCLUDE_DIRS src src/utility src/GUI src/Fonts ${EPD_DEMO_PATH}
    REQUIRES esp_common freertos hal log nvs_flash driver esp_timer arduino-audio-tools
)

target_compile_options(${COMPONENT_LIB} PUBLIC -DESP32=1 -DESP32_CMAKE=1 -Wno-error -Wno-format -fpermissive)

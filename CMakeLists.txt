cmake_minimum_required(VERSION 3.13)

set(PICO_BOARD pico_w CACHE STRING "Board type")

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(DebugThing C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(PICO_NO_PICOTOOL ON CACHE BOOL "Disable picotool requirement")

pico_sdk_init()

add_executable(debug_thing
    src/main.c
    src/utils.c
)

target_link_libraries(debug_thing
    pico_stdlib
    pico_cyw43_arch_none
    hardware_adc
)

pico_add_extra_outputs(debug_thing)
pico_enable_stdio_usb(debug_thing 1)
pico_enable_stdio_uart(debug_thing 0)

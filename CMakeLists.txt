cmake_minimum_required(VERSION 3.17)

include(pico_sdk_import.cmake)
project(usb_host)

# init Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(usb_host src/main.cpp src/EPaperDisplay.cpp)

target_link_libraries(usb_host pico_stdlib hardware_spi)

pico_add_extra_outputs(usb_host)

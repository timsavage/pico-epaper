file(GLOB FILES main.cpp **/*.cpp)

add_executable(usb_host ${FILES})

target_link_libraries(usb_host pico_stdlib hardware_spi)
pico_add_extra_outputs(usb_host)

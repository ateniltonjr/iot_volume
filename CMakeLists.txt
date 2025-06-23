cmake_minimum_required(VERSION 3.13)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(PICO_BOARD pico_w) # Define Pico W como placa alvo
include(pico_sdk_import.cmake)
project(iot_volume C CXX ASM)
pico_sdk_init()

add_executable(${PROJECT_NAME}
    iot_volume.c
    lib/ssd1306.c
    generated/ws2812.pio
)

<<<<<<< Updated upstream
add_executable(${PROJECT_NAME}  
        iot_volume.c #Principal
        lib/ssd1306.c
        lib/matrixws.c)
=======
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR})
>>>>>>> Stashed changes

target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    pico_cyw43_arch_lwip_poll # Para Wi-Fi com lwIP
    hardware_gpio
    hardware_i2c
    hardware_adc
    hardware_pio
    hardware_pwm
)

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

pico_add_extra_outputs(${PROJECT_NAME})
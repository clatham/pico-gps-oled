cmake_minimum_required(VERSION 3.10)


set(PICO_BOARD pico_w)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)


project(pico-gps-oled C CXX ASM)


pico_sdk_init()


if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "In-source builds are not allowed.  Change directory to a different location and build again.")
endif()


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)


add_executable(pico-gps-oled
    main.c
    ../thirdparty/pico-ssd1306-spi/ssd1306.c
)

target_include_directories(pico-gps-oled PRIVATE
    ${CMAKE_SOURCE_DIR}/../thirdparty
    ${CMAKE_SOURCE_DIR}/../thirdparty/pico-ssd1306-spi
)

target_link_libraries(pico-gps-oled
    pico_stdlib
    hardware_spi
)

if(PICO_CYW43_SUPPORTED)
target_link_libraries(pico-gps-oled
    pico_cyw43_arch_none
)
endif()


pico_add_extra_outputs(pico-gps-oled)
pico_enable_stdio_usb(pico-gps-oled 1)
pico_enable_stdio_uart(pico-gps-oled 0)

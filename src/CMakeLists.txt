add_executable(PicoPong
    main.c
    )

# by default the header is generated into the build dir
pico_generate_pio_header(PicoPong ${CMAKE_CURRENT_LIST_DIR}/PicoPong.pio)

target_link_libraries(
    PicoPong
    pico_stdlib
    hardware_pio
    hardware_dma
)

pico_add_extra_outputs(PicoPong)

pico_enable_stdio_usb(PicoPong 1)
pico_enable_stdio_uart(PicoPong 0)
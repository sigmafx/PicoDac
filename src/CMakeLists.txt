add_executable(PicoDac
    main.cpp
    waveWriter.cpp
    )

# by default the header is generated into the build dir
pico_generate_pio_header(PicoDac ${CMAKE_CURRENT_LIST_DIR}/PicoDac.pio)

target_link_libraries(
    PicoDac
    pico_stdlib
    hardware_pio
    hardware_dma
    hardware_sync
    hardware_adc)

pico_add_extra_outputs(PicoDac)

pico_enable_stdio_usb(PicoDac 1)
pico_enable_stdio_uart(PicoDac 0)
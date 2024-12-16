# This is the main binary. List your C files here.
add_executable(can
    can.c
    )

pico_set_program_name(can "can")
pico_set_program_version(can "0.1")

# Modify the below lines to enable/disable output over UART/USB.
# These are mutually exclusive, only set one.
pico_enable_stdio_uart(can 0)
pico_enable_stdio_usb(can 1)

# Add the libraries to the build
target_link_libraries(can PRIVATE
  pico_stdlib
  pico_multicore
  pico_async_context_freertos
  FreeRTOS-Kernel-Heap4
  can2040
)

# The CYW43 is the Wifi/Bluetooth module. If the board is set to pico_w, this
# variable will be set and the wireless libraries added.
if(PICO_CYW43_SUPPORTED)
    # For led support on pico_w
    target_link_libraries(can PRIVATE
        pico_cyw43_arch_none
    )
endif()

# Another pico macro which generates the uf2 file in addition to the elf
pico_add_extra_outputs(can)

add_custom_target(flash picotool load -f $<TARGET_FILE:can> DEPENDS can)


##
## receiver
##

# This is the main binary. List your C files here.
add_executable(receiver
    receiver.c
    )

pico_set_program_name(receiver "receiver")
pico_set_program_version(receiver "0.1")

# Modify the below lines to enable/disable output over UART/USB.
# These are mutually exclusive, only set one.
pico_enable_stdio_uart(receiver 0)
pico_enable_stdio_usb(receiver 1)

# Add the libraries to the build
target_link_libraries(receiver PRIVATE
  pico_stdlib
  pico_multicore
  pico_async_context_freertos
  FreeRTOS-Kernel-Heap4
  can2040
)

# The CYW43 is the Wifi/Bluetooth module. If the board is set to pico_w, this
# variable will be set and the wireless libraries added.
if(PICO_CYW43_SUPPORTED)
    # For led support on pico_w
    target_link_libraries(receiver PRIVATE
        pico_cyw43_arch_none
    )
endif()

# Another pico macro which generates the uf2 file in addition to the elf
pico_add_extra_outputs(receiver)

add_custom_target(flash_recieveonly picotool load -f $<TARGET_FILE:receiver> DEPENDS receiver)


##
## sender
##

# This is the main binary. List your C files here.
add_executable(sender
        sender.c
    )

pico_set_program_name(sender "sender")
pico_set_program_version(sender "0.1")

# Modify the below lines to enable/disable output over UART/USB.
# These are mutually exclusive, only set one.
pico_enable_stdio_uart(sender 0)
pico_enable_stdio_usb(sender 1)

# Add the libraries to the build
target_link_libraries(sender PRIVATE
  pico_stdlib
  pico_multicore
  pico_async_context_freertos
  FreeRTOS-Kernel-Heap4
  can2040
)

# The CYW43 is the Wifi/Bluetooth module. If the board is set to pico_w, this
# variable will be set and the wireless libraries added.
if(PICO_CYW43_SUPPORTED)
    # For led support on pico_w
    target_link_libraries(sender PRIVATE
        pico_cyw43_arch_none
    )
endif()

# Another pico macro which generates the uf2 file in addition to the elf
pico_add_extra_outputs(sender)

add_custom_target(flash_highprior picotool load -f $<TARGET_FILE:sender> DEPENDS sender)
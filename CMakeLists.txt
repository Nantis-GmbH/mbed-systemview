add_library(mbed-systemview INTERFACE)

set(SYSTEMVIEW_SOURCE_PATH SystemView_Src_V330)

target_sources(
    mbed-systemview
    INTERFACE
        ${SYSTEMVIEW_SOURCE_PATH}/SEGGER/SEGGER_SYSVIEW.c
        ${SYSTEMVIEW_SOURCE_PATH}/SEGGER/SEGGER_RTT.c
        ${SYSTEMVIEW_SOURCE_PATH}/SEGGER/SEGGER_RTT_ASM_ARMv7M.S
)

target_include_directories(
    mbed-systemview
    INTERFACE
        ${SYSTEMVIEW_SOURCE_PATH}/SEGGER
        ${SYSTEMVIEW_SOURCE_PATH}/Config
        .
)

target_link_libraries(
    mbed-systemview
    INTERFACE
        mbed-baremetal
)

# Core should not sleep when using SystemView
target_compile_options(
    mbed-systemview
    INTERFACE
        -UDEVICE_SLEEP
)
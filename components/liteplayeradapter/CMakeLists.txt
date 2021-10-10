set(COMPONENT_REQUIRES)
set(COMPONENT_PRIV_REQUIRES liteplayercore sysutils driver tone_partition)

set(COMPONENT_ADD_INCLUDEDIRS "include")

set(COMPONENT_SRCS
    source_httpclient_wrapper.c
    source_esp32_flashtone_wrapper.c
    sink_esp32_i2s_wrapper.c
)

register_component()

#target_compile_options(${COMPONENT_TARGET} PRIVATE -DOS_RTOS -DOS_FREERTOS_ESP8266)

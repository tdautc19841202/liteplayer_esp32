set(COMPONENT_REQUIRES)
set(COMPONENT_PRIV_REQUIRES sysutils)

set(COMPONENT_ADD_INCLUDEDIRS "include")

set(COMPONENT_SRCS liteplayer_debug.c)

register_component()

target_compile_options(${COMPONENT_TARGET} PRIVATE -DOS_RTOS -DOS_FREERTOS_ESP8266)

IF (CONFIG_IDF_TARGET STREQUAL "esp32")
set(libs_target_dir "esp32")
ENDIF ()
message(STATUS "${CMAKE_CURRENT_SOURCE_DIR}, Chip is ${libs_target_dir}")

IF (CONFIG_IDF_TARGET MATCHES "esp32")
target_link_libraries(${COMPONENT_TARGET} "-L ${CMAKE_CURRENT_SOURCE_DIR}/lib/${libs_target_dir}")
ENDIF ()

IF (IDF_VER MATCHES "v4.")
add_prebuilt_library(liteplayer_core
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/${libs_target_dir}/libliteplayer_core.a"
    PRIV_REQUIRES liteplayer_core sysutils)
target_link_libraries(${COMPONENT_TARGET}
    "-Wl,--start-group" liteplayer_core "-Wl,--end-group")
ELSE ()
target_link_libraries(${COMPONENT_TARGET}
    "-Wl,--start-group" liteplayer_core "-Wl,--end-group")
ENDIF ()

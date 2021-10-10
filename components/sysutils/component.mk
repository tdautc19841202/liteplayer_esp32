#
# Component Makefile
#
# This Makefile should, at the very least, just include $(SDK_PATH)/Makefile. By default,
# this will take the sources in the src/ directory, compile them and link them into
# lib(subdirectory_name).a in the build directory. This behaviour is entirely configurable,
# please read the SDK documents if you need to do this.
#

SRC_DIR := sysutils

COMPONENT_ADD_INCLUDEDIRS := \
    ${SRC_DIR}/include

COMPONENT_SRCDIRS := \
    ${SRC_DIR}/osal/esp8266 \
    ${SRC_DIR}/source/cutils \
    ${SRC_DIR}/source/cipher \
    ${SRC_DIR}/source/httpclient \
    ${SRC_DIR}/source/json

CFLAGS += -DOS_RTOS -DOS_FREERTOS_ESP8266 -DENABLE_HTTPCLIENT_MBEDTLS

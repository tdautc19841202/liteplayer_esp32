#
# Component Makefile
#
# This Makefile should, at the very least, just include $(SDK_PATH)/Makefile. By default,
# this will take the sources in the src/ directory, compile them and link them into
# lib(subdirectory_name).a in the build directory. This behaviour is entirely configurable,
# please read the SDK documents if you need to do this.
#

COMPONENT_ADD_INCLUDEDIRS := ./include

COMPONENT_SRCDIRS := .

COMPONENT_OBJS := \
    source_httpclient_wrapper.o \
    source_file_wrapper.o \
    source_esp32_flashtone_wrapper.o \
    sink_esp32_i2s_wrapper.o

#CFLAGS += -DOS_RTOS -DOS_FREERTOS_ESP8266

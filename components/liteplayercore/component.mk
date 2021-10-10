
# "main" pseudo-component makefile.

# (Uses default behaviour of compiling all source files in directory, adding 'include' to include path.)

COMPONENT_ADD_INCLUDEDIRS := include

COMPONENT_SRCDIRS := .

LIBS := liteplayer_core 

COMPONENT_ADD_LDFLAGS += -L$(COMPONENT_PATH)/lib/esp32 $(addprefix -l,$(LIBS))

ALL_LIB_FILES += $(patsubst %,$(COMPONENT_PATH)/%/lib/esp32/lib%.a,$(LIBS))

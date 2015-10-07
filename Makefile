# ARDDUDE_DIR is defined by eclipse CDT config
$(info ARDDUDE_DIR=${ARDDUDE_DIR})

all: bin

include ${ARDDUDE_DIR}/etc/tools.mk

CALLER_DIR := $(call truepath,$(dir $(firstword ${MAKEFILE_LIST})))
$(info CALLER_DIR = ${CALLER_DIR})

UPLOAD_PORT ?= /dev/ttyACM0
TARGET_BOARD ?= uno

ifneq (${MAIN_SOURCE},)
  override MAIN_SOURCE := $(subst ${CALLER_DIR}/,,${MAIN_SOURCE})
  SOURCE_DIRS := $(dir ${MAIN_SOURCE})

  SOURCE_EXCLUDE_PATTERNS := .ino
endif

include ${ARDDUDE_DIR}/etc/main.mk

ARD_TOOLS_DIR := $(call truepath,../ArduinoTools)
#LIBRARIES_DIRS := ${ARD_TOOLS_DIR}
INCLUDE_FLAGS_EXTRA += $(addprefix -I,${ARD_TOOLS_DIR} .)
LDFLAGS_EXTRA += -L${ARD_TOOLS_DIR}/target/${TARGET} -lArduinoTools

## exemples :
# make SOURCE_DIRS=alternate
# MAIN_SOURCE=alternate/firmware.cpp
# TARGET_BOARD=uno ARDUINO_IDE=/opt/arduino-1.6.0/
# UPLOAD_PORT=/dev/ttyACM0 console

# make SOURCE_DIRS=native
# MAIN_SOURCE=native/native.cpp
# TARGET_BOARD=uno ARDUINO_IDE=/opt/arduino-1.6.0/
# UPLOAD_PORT=/dev/ttyACM0 bin

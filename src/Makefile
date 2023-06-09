##
# Makefile for trampoline build
#
# @author Akafael
# @version 1.1
#
##

###############################################################################
# Variables
###############################################################################

# Get Makefile directory
MAKEFILE_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

# Trampoline paths
TRAMPOLINE_DIR=${MAKEFILE_DIR}/../lib/trampoline
GOIL_TEMPLATE_DIR=${TRAMPOLINE_DIR}/goil/templates/
GOIL_FOLDER=${TRAMPOLINE_DIR}/goil/makefile-unix
GOIL_BIN=${GOIL_FOLDER}/goil

GOIL_TARGET=avr/arduino/mega

# Arduino
TRAMPOLINE_ARDUINO_DIR=${TRAMPOLINE_DIR}/machines/avr/arduino

# MCP_CAN lib
MCP_CAN_LIB_DIR=${TRAMPOLINE_ARDUINO_DIR}/libraries/mcp_can
MCP_CAN_LIB_URL=https://github.com/akafael/MCP_CAN_lib.git

# Project names
PROJECT_NAME=trampoline_blink
PROJECT_BIN_NAME=trampuinoApp

# Project Source Files
PROJECT_DIR=${MAKEFILE_DIR}/${PROJECT_NAME}
PROJECT_SRC=${PROJECT_DIR}/${PROJECT_NAME}.cpp
PROJECT_OIL=${PROJECT_DIR}/${PROJECT_NAME}.oil
PROJECT_HEX=${PROJECT_DIR}/${PROJECT_BIN_NAME}.hex

# SETTINGS
TARGET_PORT=/dev/ttyUSB0


###############################################################################
# Rules
###############################################################################

target: ${PROJECT_HEX}

# Goil Files rules ------------------------------------------------------------

# Build goil files
.PHONY: goil_build
goil_build: ${PROJECT_OIL} ${PROJECT_SRC} install_goil install_arduino_lib
	cd ${PROJECT_DIR} &&\
	${GOIL_BIN} --target=${GOIL_TARGET} --templates=${GOIL_TEMPLATE_DIR} $(notdir $<)

# Generated build scripts from goil source file
${PROJECT_DIR}/build.py: goil_build
${PROJECT_DIR}/make.py: goil_build

# Project Build rules ---------------------------------------------------------

# Build Project
.PHONY: build
build: ${PROJECT_HEX}

${PROJECT_HEX}: goil_build
	cd ${PROJECT_DIR} &&\
	${PROJECT_DIR}/make.py

# Trampoline Install rules ----------------------------------------------------

# Install Goil
.PHONY: install_goil
install_goil: ${GOIL_BIN}

${GOIL_BIN}: ${GOIL_FOLDER}/build.py
	cd ${GOIL_FOLDER} &&\
	python3 ${GOIL_FOLDER}/build.py

# Install Arduino Core Lib
.PHONY: install_arduino_lib
install_arduino_lib: ${TRAMPOLINE_ARDUINO_DIR}

${TRAMPOLINE_ARDUINO_DIR}:${TRAMPOLINE_DIR}
	cd ${TRAMPOLINE_DIR} &&\
	git submodule init &&\
	git submodule update --init --recursive machines/avr/arduino

# Install Arduino lib MCP_CAN
.PHONY: install_mcp_can_lib
install_mcp_can_lib: ${MCP_CAN_LIB_DIR}

${MCP_CAN_LIB_DIR}: install_arduino_lib
 	git clone ${MCP_CAN_LIB_URL} ${MCP_CAN_LIB_DIR}


# Upload ----------------------------------------------------------------------

# Upload trampoline code to target
.PHONY: upload
upload: ${PROJECT_DIR}/make.py build
	cd ${PROJECT_DIR} &&\
	export AVRDUDE_PORT="${TARGET_PORT}"; ${PROJECT_DIR}/make.py flash

# General rules ---------------------------------------------------------------

# Remove Generated Files
.PHONY: clean
clean:
	cd ${PROJECT_DIR} &&\
    ${PROJECT_DIR}/make.py clean
	rm -rvf ${PROJECT_DIR}/make.py ${PROJECT_DIR}/build.py \
	   ${PROJECT_DIR}/${PROJECT_NAME} \
	   ${PROJECT_DIR}/${PROJECT_BIN_NAME}.hex \
	   ${PROJECT_DIR}/${PROJECT_BIN_NAME}

# Print help for Makefile commands
.PHONY: help
help:
	@echo "Use: make -f Makefile [OPTION]"
	@echo "\nOPTIONS"
	@sed Makefile -n -e "N;s/^# \(.*\)\n.PHONY:\(.*\)/ \2:\1/p;D" | column -ts:
	@echo ""

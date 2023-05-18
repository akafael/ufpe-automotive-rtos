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
GOIL_BIN=${TRAMPOLINE_DIR}/goil/makefile-unix/goil

GOIL_TARGET=avr/arduino/uno

#PROJECT_NAME=trampoline_blink
#PROJECT_DIR=${TRAMPOLINE_DIR}/examples/avr/arduinoUno/${PROJECT_NAME}
PROJECT_NAME=trampoline_blink
PROJECT_DIR=${MAKEFILE_DIR}/${PROJECT_NAME}
PROJECT_SRC=${PROJECT_DIR}/${PROJECT_NAME}.cpp
PROJECT_OIL=${PROJECT_DIR}/${PROJECT_NAME}.oil
PROJECT_HEX=${PROJECT_DIR}/${PROJECT_NAME}.hex

###############################################################################
# Rules
###############################################################################

target: ${PROJECT_HEX}

${PROJECT_DIR}/make.py: ${PROJECT_OIL} ${PROJECT_SRC}
	cd ${PROJECT_DIR} &&\
	${GOIL_BIN} --target=${GOIL_TARGET} --templates=${GOIL_TEMPLATE_DIR} $(notdir $<)

${PROJECT_HEX}: ${PROJECT_DIR}/make.py
	cd ${PROJECT_DIR} &&\
	$<

clean: ${PROJECT_DIR}/make.py
	cd ${PROJECT_DIR} && $< clean
	rm ${PROJECT_DIR}/${PROJECT_NAME} ${PROJECT_DIR}/*.hex -rvf

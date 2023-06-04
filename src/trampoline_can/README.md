# MCP CAN with Trampoline

Basic Application for comunication using can module.

## Build

First compile your application (oil+compilation):

```sh
goil --target=avr/arduino/uno --templates=../../lib/trampoline/goil/templates/ trampoline_can.oil
```
then compilation:
```
./make.py
```

Then upload the example (simple blink task):
```sh
export AVRDUDE_PORT="/dev/ttyACM0"
./make.py flash
```

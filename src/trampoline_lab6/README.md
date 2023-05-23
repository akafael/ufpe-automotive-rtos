# Produce Consumer Example

## Compile

First compile your application (oil+compilation):

```sh
goil --target=avr/arduino/uno --templates=../../lib/trampoline/goil/templates/ trampoline_blink.oil
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

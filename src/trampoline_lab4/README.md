# Multiple tasks with Serial Example

## Requirements


| Task | Pri |  type  | Period | Description                  |
|------|-----|--------|--------|------------------------------|
|  T1  |  5  | cyclic |   500  | Print how often was executed |
|  T2  | 10  | cyclic |   750  | Print how often was executed |
|  T3  | 15  | cyclic |  1200  | Print how often was executed |
|  T4  | 99  | cyclic |  1500  | Print all other task state.  |

## Setup

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

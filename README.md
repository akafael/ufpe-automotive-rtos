# ufpe_automotive_rtos
Class Project

## Requirements
 * Arduino
 * [TrampolineRTOS](https://github.com/TrampolineRTOS/trampoline)
 * [MCP CAN lib](https://github.com/akafael/MCP_CAN_lib)

### Ubuntu / WLS

Install dependences
```bash
sudo apt install git
sudo apt install python3 python-is-python3
sudo apt install build-essential
sudo apt install avr-libc gcc-avr avrdude
```

Build project
```bash
cd src &&\
make
```
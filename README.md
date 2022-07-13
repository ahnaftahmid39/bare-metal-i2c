# Bare Metal I2C
An implementation of using bare metal code to read temperature, pressure data from a BMP280 sensor using I2C

## Required programs/libraries:
- arm-none-eabi-gcc
- arm-none-eabi-newlib
- arm-none-eabi-binutils
- openocd
- makefile
- any serial monitor

## PIN configurations
Use these PIN configurations:
- Connect GPIOB7 pin -> SDA of BMP sensor
- Connect GPIOB8 pin -> SCL of BMP sensor
- GND -> SDO of BMP
- GND -> GND of BMP
- VCC -> VCC of BMP

## Compile

Go to terminal and run
```
make allclean
```

## Flash and Debugging:
I used `openocd` to connect to STM32 board. Then used `telnet` client to connect to `openocd` server.
Terminal Commands:
Running OpenOcd:
```
make load
```
Connecting to OpenOcd:
```
telnet localhost 4444
```

After that just send commands to talk to the board.

### Halting
```
telnet> halt
```
### Flashing
```
telnet> flash write_image erase final.elf
```
### Resetting
```
telnet> reset
```

Ignore any warning.

After making changes to code just do `make allclean` and halt, flash, reset in order.

## UART between PC and MCU
I have also connected pc with mcu using usart2. I used `CuteCom` as my serial port monitor. Baud rate used = 115200

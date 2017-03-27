# UART to USB Keyboard

The purpose of this is using a PSoC 5LP as a USB keyboard over UART. If you don't have a USB keyboard at hand, you can simply use any laptop instead.

### ToDo:
- implement key combinations (e.g. ctrl-c)
- get backspace working
- fix layout problems (y-z, special characters etc.)

Note, that the keyboard scan codes header file isn't complete. Many values are set to 0x00.
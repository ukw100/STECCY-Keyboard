# STECCY-Keyboard
## Convert your ZX-Spectrum keyboard into a PS/2 keyboard for STECCY

<img align="right" width=20% src="https://github.com/ukw100/STECCY-Keyboard/raw/main/images/steccy-keyboard-ps2.png">

The variant of connecting a ZX Spectrum keyboard as a matrix via 13 cables to the [STECCY board](https://github.com/ukw100/STECCY) has a serious disadvantage: the display together with the blackboard cannot be mounted far away from the ZX keyboard.

For this reason, there is an alternative: The ZX Spectrum keyboard is converted to a PS/2 keyboard. Then it can be connected to the BlackBoard via a longer thin PS/2 cable.

<img align="right" width=20% src="https://github.com/ukw100/STECCY-Keyboard/raw/main/images/steccy-keyboard-ps2.jpg">

Here the rows/columns are connected to the STM32F103 on a BluePill board as follows: 

```
              Columns                           Lines
              --- --- --- --- ---               --- --- --- --- --- --- --- ---
Z80           D0  D1  D2  D3  D4                A11 A10 A9  A12 A13 A8  A14 A15
STM32F103     PB3 PB4 PB5 PB6 PB7               PA3 PA2 PA1 PA4 PA5 PA0 PA6 PA7
```

Pin PB8 serves as an additional column for extensions. Again, an additional button can be placed between PB8 and PA3 to start the STECCY menu.

<img align="right" width=20% src="https://github.com/ukw100/STECCY-Keyboard/raw/main/images/steccy-ps2-female-connector-front.png">

| PS/2  | STM32F103 | Remarks                 |
|:------|:---------:|:------------------------|
| CLOCK | PB12      | pulled up on STM32F407  |
| DATA  | PB13      | pulled up on STM32F407  |
| GND   | GND       |                         |
| 5V    | 5V        |                         |

# STM32L4-ILI9341

The project is created in order to connect 320x240 LCD based on ILI9341 chip
to the STM32L476 discovery kit. It illustrates 2 important ideas:

1. Using unmodified mbed code in pure HAL based application. The specially designed hook classes
(located in '/compat' directory) serve as mbed API replacement for the ILI9341 driver code located in
'/third' directory. 

2. Extending mbed API by routine for data buffer transmitting allow us to increase LCD
refresh rate to about 27 fps which is close to the absolute maximum (32 fps at 40 Mbps).

For the LCD wiring see /Core/Inc/lcd.h

Colour OLED Breakout Board for Arduino
=======================

To use this library, copy the SSD1351 folder to the "libraries" folder of the Arduino package.

There are three different libraries that can be used for the Arduino.

1) Direct Drawing
2) Line Buffered
3) U8glib

1) Direct Drawing
Direct drawing is as it says. Every draw command you call will directly send the bytes to the display.

This method is the easiest to understand and use however, can be quite slow as you could be writing multiple bytes to a certain pixel which isn't required.

2) Line Buffered
Line buffered requires a draw loop which stores all draw calls into a buffer and sends each line of pixels to the display.
By doing this, the same amount of data is sent to the display (Cursor setup (7) + (128 x * 128 y * 3 rgb)).

This method is a little harder to use as you require a draw loop and a state-like system.

3) U8glib
The U8glib (https://code.google.com/p/u8glib/) can be used for using a vast range of fonts.
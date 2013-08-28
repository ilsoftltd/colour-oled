Colour OLED Breakout Board for Raspberry Pi
=======================
To start using the Colour OLED Breakout Board for Raspberry Pi you have two options:

Using FBTFT
=======================
To use FBTFT, please follow the "Installing FBTFT.pdf".

If any issues crop up, please check the forum at http://forum.ilsoft.co.uk for more help.

Using Code
=======================
To use the code version, you first need to download the WiringPi library from: https://projects.drogon.net/raspberry-pi/wiringpi/download-and-install/

Once you have WiringPi, you now have to make the Colour OLED library. First navigate to the "raspberryPi/ColourOLED" folder and then type "make" and press enter.

This should now compile and install the library. This library is used in the exact same way as the Arduino so please refer to the Arduino examples folder.

When compiling your code you must use -lcolourOled and -lwiringPi as a flag on the gcc or g++ complier.

NOTE: When running your program, you MUST run it as sudo. This is due to WiringPi.
Bluetooth Interfacing with Arduino
===================================

This repository contains the code examples used for the demo on 25th July 2014, @ Arduino Karkhana .


Code structure
--------------

The Python-Code directory contains Python code that can be replaced with any other computer / embedded application code, which acts like a master and seeks out the HC-05 client. 

The Arduino-Code directory contains code that can be used to interface with HC-05 using UART over the pins 0 and 1. There is also a lookup table in lookup.h to map each character of the English alphabet with an equivalent Morse signal code. 


Hardware
--------

An Arduino Uno, a HC-05 Bluetooth module, an LED and a buzzer were used for the demo.

License
-------

This work is released under Creative Commons CC-BY-NC-SA unless otherwise noted. 

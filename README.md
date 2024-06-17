[![PlatformIO CI](https://github.com/ModischFabrications/EasyLight_Code/actions/workflows/pio.yml/badge.svg)](https://github.com/ModischFabrications/EasyLight_Code/actions/workflows/pio.yml)

# EasyLight_Code
A generic firmware for [EasyLight PCB](https://github.com/ModischFabrications/EasyLight_PCB/). 

It allows control of connected RGBW Leds to give different outputs based on the three buttons (brightness, state change, extended state behaviours). 

Notable states are solid colors, pulsing or a simple fire animation. There might be more in future, check /src/modes for the current list. 

## Usage
Press the first button to step through the brightness steps. 
Press the second button to change the state, e.g. to change from solid colors to pulsing or fire. 
Press the third one to trigger state-specific actions, e.g. change color or reignite fire.

## Development
It's a generic PlatformIO project without any specials, go look there or at the refs for setup instructions.

This project is meant to be run on an Attiny85, but there is a dev environment configured for an Arduino Nano. 

## Dependencies 
See platformio.ini for details, should be handled automatically.

1. https://github.com/bxparks/AceButton
2. https://github.com/adafruit/Adafruit_NeoPixel


## References

1. https://learn.adafruit.com/multi-tasking-the-arduino-part-3/
2. https://github.com/ModischFabrications/ModischMatrix
3. https://github.com/ModischFabrications/Verdandi_Code


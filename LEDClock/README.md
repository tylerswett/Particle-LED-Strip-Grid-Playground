# Particle NeoPixel Grid Clock

- This is a work in progress \*
  Display various clock types on a Neopixel grid through a Particle microcontroller

** Please note, this project was designed for use on Particle Arduino boards and utilizies the Particle.io platform for Internet communication to the Arduino. As it is Arduino C code, it could be modified for other Arduino boards, but you would have to remove Particle specific function exposures. **

![Example of LED clock in action](/images/board.jpg)

## Setup

- Build LED matrix using individually addressable LED light strips or a prebuilt matrix
  - Your Particle board will likely not be able to drive all LEDs in your matrix, so use a good external power supply
- Connect communication wire to D6 (or any other digital port, just change PIXEL_PIN to the correct pin)
- Create a new project in the Particle IDE
- Copy LEDClock.ino into the IDE
- Under Libraries, search for Adafruit_GFX, netomatrix, and neopixel and import them
- Modify the Adafruit_NeoMatrix initialization function to match the height, width, and setup of your matrix
- Modify `Time.zone(-6)` and `Time.beginDST();` in your setup function to set your timezone and daylight savings time
- Compile and deploy to board

## Standard Configuration

In the standard configuration, on boot, a clock with a pulsing rainbow background will be displayed.

Time is obtained from the onboard clock that gets sync'd with internet time on boot.

Modes:

- Party Clock - invoked by calling showPartyClock function
  - Default boot behavior, shows a clock with a pulsing rainbow background
- Regular Clock - invoked by calling showRegularClock function
  - Shows a white clock on a blank background
  - Can modify the foreground and background colors
    - Foreground - Modify color values on lines 135-138 `matrix.Color([r], [g], [b])` (0-255 for each rgb value)
    - Background - Modify color value on line 115 `matrix.Color([r], [g], [b])` (0-255 for each rgb value)
- Alarm - invoked by calling showAlarm function
  - Flashes a bright red light
  - Can modify color and timing of flash
    - Color - Modify color value on line 152 `matrix.Color([r], [g], [b])` (0-255 for each rgb value)
    - Timing - Reuses already incrementing WheelPos for rainbow flashing to time flashes
      - Adjust Wheelpos values for on and off on line 150
      - Each value in WheelPos represents an ~10 ms segment of time
      - WheelPos loops from 0->255
      - Enable at whatever values from 0-255 you want the light to flash
- Blank - invoked by calling showNothing function

## Control

The code exposes Particle functions for each mode defined above, so you can either call those functions using curl, write a web UI, or use IFTTT integration to control it.

IFTTT integration is a great option as you can use voice commands to change the functionality of the clock or incorporate the alarm into your wakeup routine!

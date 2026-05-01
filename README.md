# pico-gps-oled

## Overview

In this project, we will connect a GNSS (GPS + GLONASS + Beidou) module and an OLED display to a Raspberry Pi Pico and program it with the C/C++ SDK for Pico.  We'll touch on the following subjects:

* Setting up our build environment
* CMake
* BASH scripting
* NMEA 0183 format for GNSS information
* Writing/drawing on an OLED
* Putting it all together to create a device that displays your geolocation


## Lessons

* [Lesson 1 - Setup an Ubuntu build environment using WSL2](./Lesson-01/Lesson-01.md)
* [Lesson 2 - Basic CMake build system](./Lesson-02/Lesson-02.md)
* [Lesson 3 - Basic BASH build script](./Lesson-03/Lesson-03.md)
* [Lesson 4 - Targeting the Pi Pico](./Lesson-04/Lesson-04.md)
* [Lesson 5 - Connecting and programming the OLED](./Lesson-05/Lesson-05.md)
* [Lesson 6 - Connecting and receiving data from the GNSS module](./Lesson-06/Lesson-06.md)
* [Lesson 7 - Parsing the NMEA 0183 output](./Lesson-07/Lesson-07.md)


## Hardware

To build this project you'll need the following components:

* Pi Pico

Get these wherever they are cheapest.  Micro Center sells them for the advertised price, as does Digikey, though you'll pay for shipping there.

* [Breadboard and Jumper Wires](https://www.amazon.com/BOJACK-Values-Solderless-Breadboard-Flexible/dp/B08Y59P6D1)

I often buy one of the Pi Pico starter kits that includes a Pi Pico and a breadboard kit, such as [this one](https://www.amazon.com/dp/B0C3771CK8).

* [OLED Display with SPI](https://www.amazon.com/dp/B08SJW4J4P)

These often come with an I2C interface, so double check you're buying one with an SPI interface.

* [GNSS Module](https://www.aliexpress.us/item/3256805966775594.html)

If you use this link, make sure you select the Neo M8N module before clicking the buy button, as several products are represented here.  The Neo M6N or Neo M7N would work as well, but the M8N has better reception under difficult circumstances.  AliExpress often offers these for $0.99 to new users, or a few dollars for users who haven't purchased in a while.  The last one I bought cost be $10, which is still a half to a third of the price of other retailers.

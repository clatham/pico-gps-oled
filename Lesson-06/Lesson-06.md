# Lesson 6 - Connecting and receiving data from the GNSS module

## Step 1 - What is a GNSS module?

Surely, you mean GPS, as in Global Positioning System.  Not quite.  GPS is the proper name of the U.S.-based, worldwide navigation system, but there are others.  There are the Russia-operated GLONASS, the EU-operated Galileo, and the Chinese-operated BeiDou.  These are referred to as Global Navigation Satellite Systems (GNSS).

The module we have is a GNSS module, because it can receive navigational data from most or all of these systems.  This can improve the ability to geolocate the module through multilateration of many satellites from multiple systems.


## Step 2 - NMEA 0183

The GNSS module (and most GPS receivers I've ever seen, including very expensive ones) output NMEA 0183 strings that contain information about the satellites being seen, ephemeris data, and position data.  This data is transmitted by a serial universal asynchronous receiver/transmitter (UART) on the GNSS module.  The default baud rate of the UART is 9600 BPS.

I'm not going to provide a full breakdown of the NMEA 0183 standard here.  Instead, please read the following:
![NMEA 0183 article on Wikipedia](https://en.wikipedia.org/wiki/NMEA_0183)

There are libraries for parsing these strings, but it's such a simple task that we're going to do it ourselves.


## Step 3 - Wiring the GNSS module

The GNSS module has five pins labelled VCC, GND, TXD, RXD, and PPS.  These are the voltage source, ground, transmit data (with respect to the module), receive data (with respect to the module), and a one pulse-per-second, respectively.  All devices need power and ground, and since we want to listen to the NMEA 0183 strings, we'll need TXD as well.

GND will be connected to the negative (-) rail of your breadboard.  VCC will be connected to the positive (+) rail, which is powered by the Pi Pico's 3.3v output.  Finally, TXD will be connected to pin 17 (GPIO 13), which is one of the UART0 RX pins.


## Step 4 - Update our main.c

Let's add defines for the UART configuration by adding the following lines above the main() function:
```
/* UART defintiions */
#define UART_ID uart0
#define BAUD_RATE 9600
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE

#define UART_TX_PIN 12
#define UART_RX_PIN 13
```

We're just defining the pins that we already talked about in the section on wiring.  Now let's use them to initialize the UART port and the GNSS module by adding the following lines to the init section of our main() function:
```
// Set up our UART with the required speed.
uart_init(UART_ID, BAUD_RATE);

// Set the TX and RX pins by using the function select on the GPIO
// Set datasheet for more information on function select
gpio_set_function(UART_TX_PIN, UART_FUNCSEL_NUM(UART_ID, UART_TX_PIN));
gpio_set_function(UART_RX_PIN, UART_FUNCSEL_NUM(UART_ID, UART_RX_PIN));

// Set UART flow control CTS/RTS, we don't want these, so turn them off
uart_set_hw_flow(UART_ID, false, false);

// Set our data format
uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);
```

First, we initialize the UART to the required baud rate.  Next, we set the TX and RX pins (we're aren't actually using the TX pin) to be used with the UART, as opposed to the other functions for which those pins *could* be used.  Next, we tell the UART that we won't be using hardware flow control (clear-to-send and ready-to-send in this case), as we've got a simple two-wire serial setup.  Lastly, we set the UART format to the ubiquitous 8n1 format (8 data bits, no parity bit, and one stop bit).

Just above our main loop, let's add the following lines:
```
char gpsLine[128];
int gpsLineOffset = 0;
```

This gives us a buffer in which to store the characters of the NMEA 0183 strings and a counter to keep track of our position in it.

Lastly, let's add the following lines in the top of our main loop:
```
while(uart_is_readable(UART_ID))
{
    uint8_t ch = uart_getc(UART_ID);
    gpsLine[gpsLineOffset++] = ch;

    if(ch == '\n')
    {
        gpsLine[gpsLineOffset] = '\0';
        printf("%s", gpsLine);

        gpsLineOffset = 0;
    }
}
```

The uart_is_readable() function returns true is the specified UART has any data in its internal first in, first out (FIFO) reaceive buffer().  While this remains true, we will fetch the next character with uart_getc(), and we'll stick it into the end of our buffer.  If the character is a new line, then we know we've reached the end of the string, so we append a NUL character (marks the end of a C-style string), print the string, and zero the offset into the buffer to begin the next string.


## Conclusion

So we're now printing the NMEA 0183 strings to Putty via the STDOUT over USB connection.  Take a look at the strings and try to understand them using the Wikipedia page linked above.  What changes when the GNSS module "locks-up" and starts flashing its red one pulse-per-second light?

Our next step will be to tokenize the strings, so that we can consume them one field at a time.  Feel free to read up on tokenization and parsing.

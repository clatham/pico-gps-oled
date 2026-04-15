# Lesson 4 - Targeting the Pi Pico

So far we've only built our "Hello, world!" application for Linux, but now it's time to target the Pi Pico itself.


## Step 1 - Include the Pico SDK

We need to modify the CMakeLists.txt to identify the version of the Pico we're using, and we need to include the SDK.  To do this, we'll add the following lines just before the project() declaration:
```
set(PICO_BOARD pico)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
```

The first line tells the Pico SDK which Pico board we're using.  The original Pico (rp2040 chipset) uses "pico" for the board without wireless capability, and it uses "pico_w" for the board with it.  The second generation Pico (rp2350 chipset) uses "pico2" for the board without wireless capability, and it uses "pico2_w" for the board with it.  You can identify the second generation Pico by the large number two silkscreened onto it.  You can identify the a Pico with wireless capability by the large, silver rectangle at one end of the board.

The second line includes the Pico SDK cmake into our build.  Notice that we're relying on the PICO_SDK_PATH environment variable that we set in Lesson 1.


## Step 2 - Init the Pico SDK

After the project() declaration, add the following line:
```
pico_sdk_init()
```

This call gives the Pico SDK an opportunity to setup whatever build resources it needs to use while building our project.


## Step 3 - Link against the SDK

We need to tell CMake to link our project against the SDK.  To do this, we're going to use a new CMake function, target_link_libraries().  Add the following lines after the add_executable() declaration:
```
target_link_libraries(pico-gps-oled
    pico_stdlib
)

if(PICO_CYW43_SUPPORTED)
target_link_libraries(pico-gps-oled
    pico_cyw43_arch_none
)
endif()
```

This tells CMake that our program requires the pico_stdlib library from the SDK.  This library provides some of the basic input/output functions and also those used to communicate with the platform environment.

We also conditional tell it to link against the pico_cyw43_arch_none library.  This is only necessary if we have a wireless capable Pico, hence the conditional.  This particular implementation doesn't actually turn on any wireless capability, hence the "none" in the name.


## Step 4 - Output the UF2 file

By default the Pico SDK is going to output an ELF file.  This is the same kind of binary that Linux uses.  However, we want the self flashing UF2 files that can be copied to the Pico.  In order to enable the output of those files, we need to add the following line:
```
pico_add_extra_outputs(pico-gps-oled)
```


## Step 5 - Review changes to main.c

Let's take a look at the updated main.c file.  First, instead of including "stdio.h", I now include "pico/stdlib.h", which is the library we told CMake to link against.  Additionally, I conditionally include "pico/cyw43_arch.h" for the wireless capabilities, if they are present.  Why do we do this?  On the regular Pico, pin 25 drives the onboard LED that we are going to flash.  However, on the W version of Pico, pin 25 drives the cyw43 wireless chip, and one of its pins in turn drives the LED.  So without the wireless capabilities enabled, you cannot flash the LED on the Pico W.

The first thing we do in the setup portion of the main() function is to call stdio_init_all().  This function initializes the standard handles that C uses, STDIN, STDOUT, and STDERR.

Next, we either initialize the GPIO pin we'll use to flash the LED on the Pico, or we let the cyw43 library do it for us on the Pico W.

Inside the loop, we set the LED state to the opposite of its current state.  This loop will run very fast on the Pico, and the LED with either look on or dim instead or flashing.  We also printf() "Hello, world!" to STDOUT.  This will flood the serial port, and it may overrun the UART buffer.


## Step 6 - Modify main.c

Modify main.c such that the infinite loop runs at 2 Hz or twice a second.  This will give the LED half a second of on time and half a second of off time.  It'll also slow down the printf() enough to not overflow the UART buffer.

Use the time_us_64() function, which returns a uint64_t containing the number of microseconds since the Pico started running.


## Step 7 - Load your program onto the Pico

You know how to build your program, but we haven't talked about how to load it onto the Pico.  Luckily, it's easy.

Holding the "bootsel" button on the Pico, plug the USB into your computer.  Because you held the button, it will mount as a USB storage device that you can see in your file system.  Copy the UF2 file from your build directory onto the Pico drive.  When the transfer is finished, the Pico will flash itself with the image and reboot.  If the LED isn't flashing, fix your code and try again.

When you plug the Pico into your USB without holding the button (or after it reboots itself), it will mount as a serial device.  This is usually COM3 on Windows, but you may need to check Device Manager to verify that.  You'll need Windows software capable of reading a serial port, and most people use Putty for this.


## Step 8 - Viewing the serial output with Putty

Download Putty from the Windows Store or from their archive here:  https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html.  You probably want the 64-bit x86 .msi file.  Install it.

Putty starts in the session configuration window.  Choose the "Serial" radio button.  Change the "Serial line" entry to the COM port that you identified earlier.  9600 is usually the appropriate baud rate for the Pico STDOUT, unless you to steps to change it.  Hit "Open" at the bottom.

If everything was successful, you know see "Hello, world!" be written out at 2 Hz from the Pico.  This is how you'll interact with or debug your programs, unless you buy a Pico debug probe.  Remember that you have to setup this Putty session every time the Pico reboots or is disconnected.


## Conclusion

You now have all of the resources to develop for your Pi Pico.  Go crazy!  Next, we'll start putting together our project.

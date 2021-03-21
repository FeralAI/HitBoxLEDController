# Hit Box LED Controller

Arduino RGB LED controller for Hit Box style controllers.

![Hit Box RGB LED Showcase](images/image000.jpg)

## Hardware

The hardware I used in my controller is:

* Seeeduino XIAO (Cortex M0+ @48MHz)
* 24x 5v WS2812 RGB LEDs (2 LEDs per button)
* Logic level converter
* Breadboard or protoboard
* 1x Tact Switch (for testing effect switching)
* A bunch of wire, solder, some connectors and zip ties
* A sacrificial USB-C cable 

![Hit Box RGB Wiring](images/image005.jpg)

Here's a wonderful text visualization of my LED chain, with `Up` being LED index 0:

```text
┌-P4--P3--P2--P1---┐
|                  |
└-K4--K3--K2--K1-┐ └-R--D--L
                 |
                 U--<-- From Arduino
```

This sketch was written for the Seeeduino XIAO and Adafruit Feather M0 boards, but should be usable on just about any Arduino compatible board with the right pin configuration. The effect change button requires a pin that supports interrupts.

These boards both run 3.3v logic, so I've used a logic level converter to boost the data line from the XIAO to 5v for the WS2812 LEDs. There are other methods to boost the data line, but I already had some converters available.

I haven't included a schematic, but wiring is straightforward:

1. Connect a button/switch to the `BUTTON_PIN` and ground for effect switching.
1. Connect `DATA_PIN`:
    * If using a 5v Arduino board:
        * Connect `DATA_PIN` to the first data pin on your LED chain.
    * If using a 3.3v Arduino board, do the following with the logic level converter:
        * Connect `DATA_PIN` to one of/the low side input.
        * Connect LED data pin/pad to the high side output.
        * Connect 3.3v supply from XIAO to 3.3v reference on low side.
        * Connect 5v supply to 5v reference on the high side.
        * Connect ground to both sides.
1. Connect LED chain 5v to your 5v supply (can pull from XIAO 5v if running via USB).
1. Connect LED chain ground to your supply ground.
1. [Optional] Add a capacitor (Adafruit recommended 1000µF, I'm using much lower) to the 5v and ground connected to your LEDs chain.
1. Power the XIAO from USB or some other 5v source (can boost a 3.3v VCC or 3.7v LiPo to 5v for this).

The USB-C cable was cut about a foot from the USB-C connector, and is directly wired to the USB power on the controller's encoder board (Brook Wireless Fighting Board) and plugged into the XIAO. If powering via a USB port on a PC, make sure you either keep the LED current draw under 500mA (50% brightness with this setup) or the USB port supports delivering the required current.

## Effects

The list of effects are:

* Static Rainbow - From Left/Red to Right/Purple
* Rainbow Chase - Moving rainbow that follows a line from Left button across the Punch buttons, wrapping right to left across Kick buttons, then down to the Up button - kind of like a question mark shape across the Hit Box layout
* Rainbow Chase Columns - Moving rainbow across columns of buttons, this is probably the moving rainbow effect you're looking for
* Static Color - Select from: White, Red, Orange, Yellow, Green, Aqua, Blue, Purple

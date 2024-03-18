# Rotary_Phone_Timer

Rotary_Phone_Timer is an Arduino Uno based project that turns a vintage rotary dial phone into a kitchen (or general purpose) timer.  

## How to Use

1) There's an on/off switch on the rear that controls all power to the Arduino and auxilliary components.

2) Numbers are input by using the rotary dial, can only enter 4 and then the display will stop updating.

3) If a mistake is made, picking up and then hanging up the headset will always reset the display inputs.

4) When the desired time as been entered, single or double press the start button to start the timer countdown.
    * **Single Press:** Min/Sec timer countdown (left 2 digits are in min, right 2 digits are in sec)
    * **Double Press:** Hr/Min timer countdown (left 2 digits are in hrs, right 2 digits are in min)

5) To shut the ringer off, pick up and then hang up the headset.

6) If you want to add time to the timer on the fly, you can dial in a number either while the countdown is in progress or when the ringer is going off. Time will always input as minutes when you do this.

## Required Components

Here's a general list of the main components required:

## Schematic
-----------------------

![Schematic](https://github.com/Scott-28/Rotary_Phone_Timer/blob/main/documentation/Schematic_Rotary-Phone-Timer_2024-03-16.png)
[PDF Version](https://github.com/Scott-28/Rotary_Phone_Timer/blob/main/documentation/Schematic_Rotary-Phone-Timer_2024-03-14.pdf)

## Software Settings

## Libraries

* TM1637Display.h -  library for controlling a TM1637 based 7-segment display, created by 'avishorp' and found here (https://github.com/avishorp/TM1637)
* Arduino.h - built in Arduino library

## Contributing

Pull requests are welcome. For major changes, please open an issue first
to discuss what you would like to change.

## License

[GNU GPLv3](https://www.gnu.org/licenses/gpl-3.0.en.html)

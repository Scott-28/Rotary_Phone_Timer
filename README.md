# Rotary_Phone_Timer

Rotary_Phone_Timer is an Arduino Uno based project that turns a vintage rotary dial phone into a kitchen (or general purpose) timer.  

<img src="https://github.com/Scott-28/Rotary_Phone_Timer/blob/main/images/overview_pic.JPG" width="500" />

* Author: Scott-28
* URL: https://github.com/Scott-28

## How to Use

1) There's an on/off switch on the rear that controls all power to the Arduino and auxilliary components.

2) Numbers are input by using the rotary dial, can only enter 4 digits and then the display will stop updating.

3) If a mistake is made, picking up and then hanging up the headset will always reset the display inputs.

4) When the desired time has been entered, single or double press the start button to start the timer countdown.
    * **Single Press:** Min/Sec timer countdown (left 2 digits are in min, right 2 digits are in sec)
    * **Double Press:** Hr/Min timer countdown (left 2 digits are in hrs, right 2 digits are in min)
      
5) The hr/min or min/sec status light will light up to tell you how much time is left.

6) To shut the ringer off, pick up and then hang up the headset.

7) If you want to add time to the timer on the fly, you can dial in a number either while the countdown is in progress or when the ringer is going off. Time will always input as minutes when you do this.

### Notes 
* The 70VAC 20Hz signal for the ringer causes a lot of noise! When it turns on, it can cause false signals in the arduino and cause the display to go blank or the handset switch interrupt to falsely trigger.
   * To combat this, a filtering capacitor was added to the handset switch which solves the noise causing the handset interrupt to falsely trigger.
   * If the noise causes the display to go blank, it is reset almost immediately since the display is constantly updated, so it's almost unperceivable.
   * If I were to redo this project, I would opt for shielded wiring for all the Arduino IO.
     
*  You'll notice the schematic has 2 grounds labeled (GND1 & GND2); the filtering capacitor MUST be grounded to GND1 (7.5VDC arduino power side) or else the partial isolation through the DC-DC converter will effect how well the capacitor does it's job at blocking noise.

*  The original network box inside the phone was completey removed, it's not needed anymore since the arduino now acts as the brains.  

## Required Components

Here's a general list of the main components required:
| Item # | Schematic # | Qty | Description |
| ----------- | ----------- | --- | ------------|
| 1 | U1 | 1 | Arduino Uno |
| 2 | U2 | 1 | TM1637 4-Digit 7 Segment Display |
| 3 | U3 | 1 | LM2596 DC-DC Step Down Converter (7.5VDC to 5VDC) |
| 4 | U4 | 1 | Power Supply (120VAC to 7.5VDC) |
| 5 | U5 | 1 | 5V Relay Module w/ Optocoupler |
| 6 | U6 | 1 | Sine Wave Generator (5VDC to 70VAC @ 20Hz) |
| 7 | U7 | 1 | Rotary Dial |
| 8 | U8 | 1 | Phone Ringer |
| 9 | D1 & D2 | 2 | LED status lights for hr/min & min/sec |
| 10 | R1 & R2 | 2 | 270 Ohm resistor for LEDs |
| 11 | SW1 | 1 | Main Power On/Off |
| 12 | SW2 | 1 | Momentary start button |
| 13 | SW3 | 1 | Phone Handset |
| 14 | C1 | 1 | 0.1uF capacitor for filtering of the handset signal |

## Schematic

![Schematic](https://github.com/Scott-28/Rotary_Phone_Timer/blob/main/docs/Schematic_Rotary-Phone-Timer_2024-03-16.png)
[PDF Version](https://github.com/Scott-28/Rotary_Phone_Timer/blob/main/docs/Schematic_Rotary-Phone-Timer_2024-03-14.pdf)

## Libraries

* TM1637Display.h -  library for controlling a TM1637 based 7-segment display, created by 'avishorp' and found here (https://github.com/avishorp/TM1637)
* Arduino.h - built in Arduino library

## Contributing

Pull requests are welcome. For major changes, please open an issue first
to discuss what you would like to change.

## License

[GNU GPLv3](https://www.gnu.org/licenses/gpl-3.0.en.html)

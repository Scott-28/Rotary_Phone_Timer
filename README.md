# Rotary_Phone_Timer

Rotary_Phone_Timer is an Arduino Uno based project that turns a vintage rotary dial phone into a kitchen (or general purpose) timer.  

<img src="images/1-Cover_Pic.jpg" width="500" />

The project started by searching for a vintage rotary phone at thrift stores. I originally had in mind a classic 50-70s style or french style phone, but then came across this very old wooden box phone 
that was originally a hand crank phone. At some point, someone converted it to a rotary phone, and now it gets to serve another life as a kitchen timer. Much more room inside to hide electronics 
than a newer rotary phone.

Originally, I was expecting to have to figure out how the 425B network box worked that ran the rotary phone, but then realized I could use the Arduino as the sole controller. I used an 
oscilloscope to figure out the pulses off the rotary dial, each pulse was ~60ms high with a 100ms period. Each number is 1 pulse and a 0 is 10 pulses. The next main challenge was how 
to operate the original ringer (I really didn't want to resort to using a speaker). In North America, phone ringers run off 60-90VDC and 20Hz. I found a small company called 
'Cambridge Electronics Laboratories' that sells 'Black Magic' ring generators of various input and output voltages. I purchased a 5VDC input sine wave generator so I could use the 
same power supply as the Arduino. This ring generator creates a lot of electrical noise and I had to add a filter capacitor to the handset input otherwise it would frequently false trigger.

One of the main goals of the project was to keep the vintage look. So I opted for a red 7-segment display and small red status lights. The labels are hand stamped brass. The switch on the back is a large metal switch with a heavy analog feel.

<u>Hopefully this project inspires others to re-purpose items and create fun arduino projects!</u>

[Click Here for the Picture Gallery](https://github.com/Scott-28/Rotary_Phone_Timer/tree/main/images)

* Author: Scott-28
* URL: https://github.com/Scott-28

## How to Use

1) There's an on/off switch on the rear that controls all power to the Arduino and auxilliary components.

2) Numbers are input by using the rotary dial, you can only enter a max of 4 digits and then the display will stop updating.

3) When the desired time has been entered, single or double press the start button to start the timer countdown.
    * **Single Press:** Min/Sec timer countdown (left 2 digits are in min, right 2 digits are in sec)
    * **Double Press:** Hr/Min timer countdown (left 2 digits are in hrs, right 2 digits are in min)
      
4) The timer will start counting down and the hr/min or min/sec status light will light up to tell you which mode the timer is in so you know how much time is left.

5) To shut the ringer off, pick up and then hang up the headset.

6) You can also add additional time on the fly! Dial in a number either while the countdown is in progress or when the ringer is going off. Time will always input as minutes when you do this (a 5 will add 5min).

    #### Use Notes
    
    * If a mistake is made while entering time, picking up and then hanging up the headset will always reset the display.
    * For a quicker input, time that's in the "min/sec" format can be entered in "hr/min" format and then a double press can be used, as long as an even number minute is desired.
        * Example 1: If you want a timer of 5min, you can simply enter a single 5 and then double press which means the timer sees 00:05 and in hr/min format a double press means 5min. The timer will switch to 05:00 and start counting down the seconds.
        * Example 2: If a time of 45 is desired, you can simply enter a 4 and a 5 (00:45) and double press and in hr/min format a double press means 45min. The timer will switch to 45:00 and tart counting down the seconds.
        
## Video Demo

<a href="https://youtu.be/joO1zkLfmjA">
<img src="images/6-YouTube_Cover.jpg">
</a>

### Build Notes  
* The 70VAC 20Hz signal for the ringer causes a lot of noise! When it turns on, it can cause false signals in the arduino and cause the display to go blank or the handset switch interrupt to falsely trigger.
   * To combat this, a filtering capacitor was added to the handset switch which solves the noise causing the handset interrupt to falsely trigger.
   * If the noise causes the display to go blank, it is reset almost immediately since the display is constantly updated, so it's almost unperceivable.
   * If I were to redo this project, I would opt for shielded wiring for all the Arduino IO.
     
*  You'll notice the schematic has 2 grounds labeled (GND1 & GND2); the filtering capacitor MUST be grounded to GND1 (7.5VDC arduino power side) or else the partial isolation through the DC-DC converter will effect how well the capacitor does it's job at blocking noise.

*  The original network box inside the phone was completey removed, it's not needed anymore since the arduino now acts as the brains.  

## Required Components

| Item # | Schematic # | Qty | Description |
| ----------- | ----------- | --- | ------------|
| 1 | U1 | 1 | Arduino Uno |
| 2 | U2 | 1 | TM1637 4-Digit 7 Segment Display |
| 3 | U3 | 1 | LM2596 DC-DC Step Down Converter (7.5VDC to 5VDC) |
| 4 | U4 | 1 | Power Supply (120VAC to 7.5VDC) |
| 5 | U5 | 1 | 5V Relay Module w/ Optocoupler |
| 6 | U6 | 1 | LS057020 Black Magic Sine Wave Generator (5VDC to 70VAC @ 20Hz) |
| 7 | U7 | 1 | Rotary Dial |
| 8 | U8 | 1 | Phone Ringer |
| 9 | D1 & D2 | 2 | LED status lights for hr/min & min/sec |
| 10 | R1 & R2 | 2 | 270 Ohm resistor for LEDs |
| 11 | SW1 | 1 | Main Power On/Off |
| 12 | SW2 | 1 | Momentary start button |
| 13 | SW3 | 1 | Phone Handset |
| 14 | C1 | 1 | 0.1uF capacitor for filtering handset signal |

Miscellaneous components not listed in table:
* Old rotary phone (this should have the rotary dial, headset, and ringer needed)
* 3D printed enclosure for the 4-digit display
* Various screws to hold components in place
* PCB prototype boards to make connecting/soldering components easier
* Miscellaneous wiring/terminal blocks/connectors to connect components

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

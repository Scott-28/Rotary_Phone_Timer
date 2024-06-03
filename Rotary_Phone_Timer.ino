#include <Arduino.h>
#include <TM1637Display.h>

/*
 * Rotary Phone Timer
 * V1.1.0
 */

//declaring pins
const byte do_RingerRelay = 11;          // digital output relay pin (to turn ringer on)
const byte do_hr_min = 10;               // digital output for hr/min LED
const byte do_min_sec = 9;               // digital output for min/sec LED
const byte di_StartButton = 12;          // digital input pin for PB to start timer
const byte di_RotaryDial = 8;            // digital input pin for rotary dial pulses
const byte di_interrupt_HandsetSw = 2;   // digital input pin for shutting off the ringer
const byte CLK = 4;                      // digital pin for the display CLK pin
const byte DIO = 5;                      // digital pin for the display DIO pin

const byte brightness = 3;               // brightness variable for display (0 is dimmest, 7 is brightest)

volatile byte inum = 1;                  // variable to count how many numbers have been input
volatile byte input_1 = 0;
volatile byte input_2 = 0;
volatile byte input_3 = 0;
volatile byte input_4 = 0;
volatile int first_two = 0; 
volatile int last_two = 0;

volatile byte start_btn_state = HIGH;    // variable for start button state
volatile byte btn_press_type = 0;        // variable for single or double start button press

volatile byte pulse_count = 0;           // variable for how many pulses the rotary dial outputs
volatile byte count = 0;                 // variable for temporarily counting pulses until a single rotary dial input sequence is done
volatile byte pulse_state = LOW;         // variable for rotary dial pulse state
volatile byte prev_pulse_state = LOW;    // variable for previous rotary dial pulse state
volatile bool firsttime = true;          // variable to know if rotary dial input is in the first pulse or not

unsigned long COUNTDOWN_TIME = 0;        // variable for time (in sec) that is inputted

volatile unsigned long pulsetime_last = millis();      //
volatile unsigned long mainloop_time = millis();       //

// Create a display object of type TM1637Display
TM1637Display display(CLK, DIO);

void setup() {
  // put your setup code here, to run once:

  pinMode(do_RingerRelay, OUTPUT);                     // declare do_RingerRelay as output
  pinMode(do_hr_min, OUTPUT);                          // declare do_hr_min as output
  pinMode(do_min_sec, OUTPUT);                         // declare do_min_sec as output
  pinMode(di_RotaryDial, INPUT_PULLUP);                // declare di_RotaryDial as pullup input
  pinMode(di_interrupt_HandsetSw, INPUT_PULLUP);       // declare di_interrupt_HandsetSw as pullup input
  pinMode(di_StartButton, INPUT_PULLUP);               // declare di_StartButton as pullup input
  attachInterrupt(digitalPinToInterrupt(di_interrupt_HandsetSw), reset, RISING);  // make di_interrupt_HandsetSw a interrupt input pin

  pinMode(13, OUTPUT);                                 // used as a visual to see if reset was triggered

  // Set baud rate for serial monitor, can de-activate later
  Serial.begin(9600);

  // Clear the display on program start-up
  display.setBrightness(brightness, false);
  display.clear(); // just using clear wasn't working properly so needed to set the brightness but turn the display off with false first

  // Set the display brightness
  display.setBrightness(brightness);
}

void loop() {
  digitalWrite(13, LOW);       // just used as a visual to see if reset was triggered, set back to low here after a reset
  display.setBrightness(brightness, true); // Needed to set display brightness back to 0 and on after the reset() function
  mainloop_time = millis();
  if (inum <= 4) {
    // Read the state of the rotary dial switch (reads HIGH when actuated)
    pulse_state = digitalRead(di_RotaryDial);
    // Set pulse count if no pulses detected
    if (((mainloop_time - pulsetime_last) >= 150) && (pulse_state == LOW)) {
      pulse_count = count;
      count = 0; // Reset count after displaying it so pulses count correctly next time rotary dial is used
      firsttime = true; //reset to true for next time the rotary dial is used
    } else if (pulse_state != prev_pulse_state) {  // Go to function to add pulse when change of pulse state is detected
      RotaryInput();
      delay(5); //Slight delay to avoid bouncing (only delay when change in pulse state is detected so not to delay every single loop thru)
    }
  }

  // Don't count pulses and update display if the first entry is a 0
  if ((inum == 1) && (pulse_count == 10)) {
    pulse_count = 0;
  } else {
    // if rotary input detected, store pulse count in correct register
    if (pulse_count > 0) {
      // Convert pulse_count of 10 into 0
      if (pulse_count == 10) {
        pulse_count = 0;
      }
      // Set the input values so each one is saved in the order inputted
      if (inum == 1) {
        input_1 = pulse_count;
      } else if (inum == 2) {
        input_2 = pulse_count;
      } else if (inum == 3) {
        input_3 = pulse_count;
      } else if (inum == 4) {
        input_4 = pulse_count;
      } 

      COUNTDOWN_TIME = UpdateDisplay(pulse_count, input_1, input_2, input_3, input_4);
      Serial.print("COUNTDOWN_TIME: ");
      Serial.println(COUNTDOWN_TIME);
      Serial.print("inum (number input): ");
      Serial.println(inum);
      pulse_count = 0; // Reset pulse count after displaying it so pulses count correctly next time rotary dial is used
      inum++; // index the inum count so input digit is in correct spot
    }
  }

  // Check for the start button being pressed
  start_btn_state = digitalRead(di_StartButton);
  // If button is pressed, begin sequence of events to start the timer
  if ((start_btn_state == LOW) && (COUNTDOWN_TIME != 0)) {
    SingleorDouble(); // Function to detect single or double button press for mode
    StartTimer(); // Function to start timer count down
  }
}

void RotaryInput() {
  if ((pulse_state == HIGH) && (firsttime == true)) { //Pulse state went from low to high AND it's first pulse of the dial
    pulsetime_last = millis();
    count++;
    firsttime = false;
  } else if ((pulse_state == HIGH) && ((mainloop_time - pulsetime_last) < 150)) { //Pulse state went from low to high but it's not the first pulse of the dial
    count++;
    pulsetime_last = millis();
  }
  prev_pulse_state = pulse_state;
}

int UpdateDisplay(int rotary_num, byte digit_1, byte digit_2, byte digit_3, byte digit_4) {
  int total_time_sec = 0;
  int total_time_mod = 0;
  // Determine number to display as numbers are input from rotary dial
  if (inum == 1) {
    total_time_sec = rotary_num;
    last_two = rotary_num;
  } else if (inum == 2) {
    total_time_sec = ((digit_1 * 10) + rotary_num);
    last_two = total_time_sec;
  } else if (inum == 3) {
    total_time_sec = ((digit_1 * 60) + (digit_2 * 10) + rotary_num);
    first_two = digit_1;
    last_two = (digit_2 * 10) + digit_3;
  } else if (inum == 4) {
    total_time_sec = (((digit_1 * 10) + digit_2) * 60) + ((digit_3 * 10) + rotary_num);
    first_two = (digit_1 * 10) + digit_2;
    last_two = (digit_3 * 10) + digit_4;
  }

  Serial.print("first_two: ");
  Serial.println(first_two);
  Serial.print("last_two: ");
  Serial.println(last_two);

  if ((last_two >= 60) && (last_two <= 99)) {
    total_time_mod = (first_two * 100) + last_two;
    display.showNumberDecEx(total_time_mod, 0b01000000, false);
  } else {
    int minutes = total_time_sec / 60;
    int seconds = total_time_sec % 60;
    total_time_mod = (minutes * 100) + seconds;
    display.showNumberDecEx(total_time_mod, 0b01000000, false);
  }
  return total_time_sec;
}

void SingleorDouble() {
  byte num_of_presses = 0;
  byte prev_start_btn_state = HIGH;       // variable for previous start button state
  unsigned long start_time = millis();
  unsigned long time = millis();
  while ((time - start_time) < 600) {
    start_btn_state = digitalRead(di_StartButton);
    //compare pulse_state to it's previous state
    if (start_btn_state != prev_start_btn_state) {
      // only increment the counter if the state went from HIGH to LOW
      if (start_btn_state == LOW) { 
        //if the current state is LOW then the start button was pressed:
        start_time = millis(); //reset this for when it goes LOW
        num_of_presses++;
      } else {
        // if the current state is HIGH then the start button went from on to off:
        time = millis();
      }
      // Slight delay to avoid bouncing
      delay(5);
    }
    // save the current state as the last state, for next time through the loop
    prev_start_btn_state = start_btn_state;
    time = millis();
  }
  btn_press_type = num_of_presses;
}

void StartTimer() {
  unsigned long start_time = millis();
  unsigned long remaining_time; // always in seconds
  unsigned long display_time = 0;
  unsigned long elapsed_time;
  int time_to_add = 0;
  
  // multiply by 60 for correct time (in sec) if hr/min mode is selected
  if (btn_press_type >= 2) {
    COUNTDOWN_TIME = (COUNTDOWN_TIME * 60) + 59; // add 59 for integer rounding down so display shows time input for first minute
    // if less than 1hr, convert back to min/sec mode
    if (COUNTDOWN_TIME <= 3659) {
      btn_press_type = 1;
      COUNTDOWN_TIME = COUNTDOWN_TIME - 60;
      Serial.println(remaining_time);
      Serial.println("DID I COME HERE??");
    }
  }

  // Turn on correct LED light for hr/min or min/sec
  if (btn_press_type >= 2) {
    digitalWrite(do_hr_min, HIGH); // turn hr/min light on
  } else {
    digitalWrite(do_min_sec, HIGH); // turn min/sec light on
  }

  remaining_time = COUNTDOWN_TIME;

  // loop through timer sequence
  while (btn_press_type != 0) {
    unsigned long current_time = millis();

    elapsed_time = (current_time - start_time) / (1000); // calculate elapsed time (in sec) since start button was pushed
    remaining_time = COUNTDOWN_TIME - elapsed_time; // in seconds
    int rem_last_two = last_two;
    rem_last_two = last_two - elapsed_time;
    //Serial.println(rem_last_two);

    // switch to min/sec mode when time reaches 1 hour
    if ((remaining_time <= 3659) && (btn_press_type >= 2)) {
      btn_press_type = 1; // ensures this is only called once
      digitalWrite(do_hr_min, LOW); // turn hr/min light off
      digitalWrite(do_min_sec, HIGH); // turn min/sec light on
      remaining_time = remaining_time - 60; // removes the added time while in hr/min mode for display
      COUNTDOWN_TIME = COUNTDOWN_TIME - 60;
    }

    // convert remaining time in seconds to the correct format for the display
    if((btn_press_type >= 2) && (last_two < 60)){ // convert seconds to hr/min
      display_time = ((remaining_time / 3600) * 100) + ((remaining_time % 3600) / 60);
      rem_last_two = -1;
      Serial.println("normal-hr");
    } else if ((btn_press_type >= 2) && (rem_last_two >= 60)) {
      display_time = (((remaining_time / 3600) * 100) - 100) + rem_last_two;
      Serial.println(">60-hr");
    } else if ((btn_press_type >= 2) && (rem_last_two >= 0)) {
      display_time = ((remaining_time / 3600) * 100) + rem_last_two;
      Serial.println("<60-hr");
    } else if ((btn_press_type == 1) && (rem_last_two >= 60)) {
      display_time = (((remaining_time / 60) * 100) - 100) + rem_last_two;
      Serial.println(">60-sec");
    } else if ((btn_press_type == 1) && (rem_last_two >= 0)) {
      display_time = ((remaining_time / 60) * 100) + rem_last_two;
      Serial.println("<60-sec");
    } else { // convert seconds to min/sec
      display_time = ((remaining_time / 60) * 100) + ((remaining_time) % 60);
      rem_last_two = -1;
      Serial.println("normal-sec");
    }
      Serial.println(rem_last_two);
    display.showNumberDecEx(display_time, 0b01000000, false);

    // Go to Ringer function when time reaches zero
    if (remaining_time == 0) {
      Ringer();
    }

    //-----------------SECTION FOR INPUTTING MORE TIME WHILE TIMER SEQUNCE IS RUNNING ---------------------------------------------------------
    mainloop_time = millis();
    // Read the state of the rotary dial switch (reads HIGH when actuated)
    pulse_state = digitalRead(di_RotaryDial);
    // Set pulse count if no pulses detected
    if (((mainloop_time - pulsetime_last) >= 150) && (pulse_state == LOW)) {
      pulse_count = count;
      count = 0; // Reset count after displaying it so pulses count correctly next time rotary dial is used
      firsttime = true; //reset to true for next time the rotary dial is used
    } else if (pulse_state != prev_pulse_state) {  // Go to function to add pulse when change of pulse state is detected
      RotaryInput();
      delay(5); //Slight delay to avoid bouncing (only delay when change in pulse state is detected so not to delay every single loop thru)
    }
    // Don't count pulses and update display if the entry is a 0 (can't add 0 time)
    if (pulse_count == 10) {
      pulse_count = 0;
    } else if (pulse_count > 0) {
      // add time in minutes
      COUNTDOWN_TIME = COUNTDOWN_TIME + (pulse_count * 60);
      pulse_count = 0; // Reset pulse count after adding it to timer so pulses count correctly next time rotary dial is used
    }
    //------------------------------------------------------------------------------------------------------------------------------------------
  }
  display.setBrightness(brightness, false);
  display.clear();
}

void Ringer() {
  unsigned long start_time = millis();
  unsigned long prev_display_time = millis();
  unsigned long prev_ring_time = millis();
  unsigned long current_time = millis();
  bool display_on = true;
  bool prev_display_on = false;
  byte ringer_on = HIGH;
  display.showNumberDecEx(0, 0b01000000, true); // Display "00:00" (turn on to start)
  digitalWrite(do_hr_min, HIGH); // turn hr/min light on (turn on to start)
  digitalWrite(do_min_sec, HIGH); // turn min/sec light on (turn on to start)
  digitalWrite(do_RingerRelay, HIGH); // Turn ringer on to start
  // Loop through alarm/ringer sequence for 30min, after 30min it will auto-shutoff (or when phone in hung-up)
  while ((btn_press_type != 0) && ((current_time - start_time) < 1800000)) {
    // Blink on and off "00:00" every 500ms 
    if ((current_time - prev_display_time) > 500) {
      if (display_on == true) {
        display.clear();
        digitalWrite(do_hr_min, LOW); // turn hr/min light off
        digitalWrite(do_min_sec, LOW); // turn min/sec light off
        display_on = false;
        prev_display_time = millis();
      } else {
        display.showNumberDecEx(0, 0b01000000, true);
        digitalWrite(do_hr_min, HIGH); // turn hr/min light on
        digitalWrite(do_min_sec, HIGH); // turn min/sec light on
        display_on = true;
        prev_display_time = millis();
      }
    }
    // Ring cadence; 2sec on, 4sec off
    if (((current_time - prev_ring_time) > 2000) && (ringer_on == HIGH)) {
      digitalWrite(do_RingerRelay, LOW); // turn ringer off
      prev_ring_time = millis();
      ringer_on = LOW;
    } else if (((current_time - prev_ring_time) > 4000) && (ringer_on == LOW)) {
      digitalWrite(do_RingerRelay, HIGH); // turn ringer off
      prev_ring_time = millis();
      ringer_on = HIGH;
    }
    current_time = millis();

    //-----------------SECTION FOR INPUTTING MORE TIME WHILE RINGER SEQUNCE IS ON ---------------------------------------------
    mainloop_time = millis();
    // Read the state of the rotary dial switch (reads HIGH when actuated)
    pulse_state = digitalRead(di_RotaryDial);
    // Set pulse count if no pulses detected
    if (((mainloop_time - pulsetime_last) >= 150) && (pulse_state == LOW)) {
      pulse_count = count;
      count = 0; // Reset count after displaying it so pulses count correctly next time rotary dial is used
      firsttime = true; //reset to true for next time the rotary dial is used
    } else if (pulse_state != prev_pulse_state) {  // Go to function to add pulse when change of pulse state is detected
      RotaryInput();
      delay(5); //Slight delay to avoid bouncing (only delay when change in pulse state is detected so not to delay every single loop thru)
    }
    // Don't count pulses and update display if the entry is a 0 (can't add 0 time)
    if (pulse_count == 10) {
      pulse_count = 0;
    } else if (pulse_count > 0) {
      // Set the inputs so UpdateDisplay() function works properly
      input_1 = pulse_count;
      input_2 = 0;
      input_3 = 0;
      input_4 = 0;
      inum = 1; // Forces this back to first input so UpdateDisplay() function works properly
      COUNTDOWN_TIME = UpdateDisplay(pulse_count, input_1, input_2, input_3, input_4);
      digitalWrite(do_RingerRelay, LOW);     // turn ringer off before going back into timer function
      digitalWrite(do_hr_min, LOW); // turn hr/min light off
      digitalWrite(do_min_sec, LOW); // turn min/sec light off
      btn_press_type = 2; // Forces timer to go into hr/min mode regardless of what mode it was originally in
      pulse_count = 0; // Reset pulse count after displaying it so pulses count correctly next time rotary dial is used
      StartTimer(); // Function to start timer again with added time to it (in minutes)
    }
    //---------------------------------------------------------------------------------------------------------------------------
  }
  reset();
}

void reset() {
  digitalWrite(13, HIGH);             // just used as a visual to see if reset was triggered
  digitalWrite(do_RingerRelay, LOW);  // turn ringer off
  display.setBrightness(brightness, false);
  display.clear();
  digitalWrite(do_hr_min, LOW);  // turn hr/min light off
  digitalWrite(do_min_sec, LOW);  // turn min/sec light off
  pulse_count = 0;
  count = 0;
  pulse_state = LOW;
  inum = 1;
  start_btn_state = HIGH;
  btn_press_type = 0;
  input_1 = 0;
  input_2 = 0;
  input_3 = 0;
  input_4 = 0;
  first_two = 0; 
  last_two = 0;
  COUNTDOWN_TIME = 0;
  firsttime = true;
  prev_pulse_state = LOW;
}
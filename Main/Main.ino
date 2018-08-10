/*
 * Main.ino
 * 
 * Author: Jonah Harris
 * Modified: 8/10/2018
 */ 
#include "Timer.h"
#include "Lock.h"
#include "Button.h"
#include "PinRegistry.h"
#include "Display.h"

Timer timer;
Lock lock(PinRegistry::LOCK);
Button button1(PinRegistry::BUTTON1);
Button button2(PinRegistry::BUTTON2);
Display display(PinRegistry::ANODES, PinRegistry::CATHODES);

// Jonah's added variables
unsigned long holdTime = 0;
int pressed = 1;
int reset = 1;
unsigned long displayTime = 0;
int off = 1;
const int flash = 500; // Time to flash
unsigned long lockTime = 0;
const int warning = 10000; // Time until lock sets
int lockFlag = 1; // When to start warning countdown

int hours;
int minutes;
int start_mn = 0;
int prev_mn = 0;
int start_hr = 0;
int prev_hr = 0;
int i = 0;

void setup()
{

	  //Serial.begin(9600);
	  display.enablePins();
	  pinMode(PinRegistry::BUTTON1, INPUT);
	  pinMode(PinRegistry::BUTTON2, INPUT);
	  pinMode(PinRegistry::LOCK, OUTPUT);
		
}

void loop()
{
	bool button1Pressed = button1.isPressed();
	bool button2Pressed = button2.isPressed();
	bool button1Held = button1.isHeld();
	bool button2Held = button2.isHeld();

	if (timer.isRunning()==1)
	{
		if (timer.ringRing()==true)
		{
			start_hr = 0;
			start_mn = 0;
			display.clearDigits();
			display.showDot(i);
			if (i<4){
				i+=1;
			} else {
				i=0;
			}

			
			lock.open();
			if (button1Held&button2Held)
			{
				timer.stop();
			}
		}
		else
		{
			timer.tic();
			hours = timer.getHours();
			int time = hours*100+timer.getMinutes();
			display.show(time);
			//display.showDot(1); // takes some juice away from the other segments
			if (timer.getMinutes()!=minutes)
			{
				minutes = timer.getMinutes();
						
			}
		}
		
		
	}
	else
	{

    // Flash screen when a time is input
    if (start_hr != 0 || start_mn != 0) {
      if (off) {
        display.clearDigits();
      } else {
        display.show(start_hr*100+start_mn);
      }

      lockFlag = 1;
      if (start_hr != prev_hr || start_mn != prev_mn) {
        lockTime = millis();
        prev_hr = start_hr;
        prev_mn = start_mn;
      }

      
      if (reset) {
        displayTime = millis();
        reset = 0;
      } 
      
      if ((millis() - displayTime > flash) && off) {  // Wait 1 second
        display.show(start_hr*100+start_mn);
        reset = 1;
        off = 0;
      } else if ((millis() - displayTime > flash) && !off) {
        display.clearDigits();
        reset = 1;
        off = 1;
      }
    } else {  // Wait until time is input
      display.show(start_hr*100+start_mn);
      lockFlag = 0;
    }

    // Lock after designated time
		if ((millis() - lockTime > warning) && lockFlag) {
			timer.setTimer(start_hr, start_mn);
			timer.start();
			lock.close();
      lockFlag = 0;
		}
    else if (button1Pressed) {
      if (start_mn==45) {
        start_hr += 1;
        start_mn = 0; 
      } else {
        start_mn += 15;       
      } 
      pressed = 1;
    }
    else if (button2Pressed)
    {
      start_hr+=1;
      pressed = 1;
    }
		else if (button1Held || button2Held || (button2Held && button1Held))
		{
      if (pressed) {
        holdTime = millis();
        pressed = 0;
      }
      if (millis() - holdTime > 3000) { // if it's been 3 seconds since holdTime was last saved.
  			start_hr = 0;
        start_mn = 0;
        pressed = 1;
      }
		}
		else {
			lock.open();
      pressed = 1;
		}

	}
	

	 
}

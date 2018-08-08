/*
 * Main.ino
 *
 * Created: 12/4/2014 9:30:28 PM
 * Author: amate_000
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
//Display display(PinRegistry::ANODES, PinRegistry::CATHODES);

int hours;
int minutes;

int wait_array[] = { 4, 50, 57, 59};  // First option is for testing
int rest_array[] = { 1, 10, 3, 1};
int ledArray[] = {3, 5, 6, 9};
int brightArray[] = {255, 255, 255, 0};
int index = 0;
int ledIndex = 3;
int fade = 5;
int brightness = 0;
int ledFlag2 = 1;     // Flags for whether an LED has been modified yet
int ledFlag1 = 1;
int ledFlag0 = 1;
int timerFlag = 1;
int dimFlag = 0;
int fadeArray[] = {5, 5, 5, 5};

int start_mn = 0;
int start_hr = 0;
int flow = 0;

void setup()
{

	  //Serial.begin(9600);
	  //display.enablePins();
	  pinMode(PinRegistry::BUTTON1, INPUT);
	  pinMode(PinRegistry::BUTTON2, INPUT);
	  pinMode(PinRegistry::LOCK, OUTPUT);
    // LED outputs:
    pinMode(3, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(9, OUTPUT);
    
		
}

void loop()
{
	bool button1Pressed = button1.isPressed();
	bool button2Pressed = button2.isPressed();
	bool button1Held = button1.isHeld();
	bool button2Held = button2.isHeld();

	if (timer.isRunning()==1)
	{
		if (timer.ringRing()==true)   // After timer rings
		{
      if (timerFlag) {
        for (int i = 0; i < 4; i++){
          brightArray[i] = 0;
          analogWrite(ledArray[i], brightArray[i]);
        }
        timerFlag == 0;
      }

      lock.open();

      int waitBetween = 0;
      while (true) {
        while (waitBetween < 25) {
          waitBetween += 1;
          analogWrite(ledArray[flow], brightArray[flow]);
          
          brightArray[flow] += fadeArray[flow];
      
          if (brightArray[flow] == 0 || brightArray[flow] == 125) {
              fadeArray[flow] = -fadeArray[flow];
          } 
          //  wait for 30 milliseconds to see the dimming effect
          delay(30);
        }
        waitBetween = 0;
        flow += 1;
        if (flow == 4) {
          flow = 0;
        }
        
      }

      
			if (button1Held&button2Held)
			{
				timer.stop();
			}
		}
		else  // Countdown section after timer preset is selected
		{
			timer.tic();
			hours = timer.getHours();
			int time = hours*100+timer.getMinutes();
			//display.show(time);
			//display.showDot(1); // takes some juice away from the other segments
			if (timer.getMinutes()!=minutes)
			{
				minutes = timer.getMinutes();
						
			}

      if (dimFlag == 300) {   // Delay appears to be messing with the timer functionality, so instead we only enter this loop every so many cycles to mimick the delayBut still have the timer work.
        dimFlag = 0;
        if (timer.getHours() == 2 && ledFlag2) {
            brightArray[2] = brightArray[3];
            brightArray[3] = 0;
            ledIndex = 2;
            ledFlag2 = 0;
         } else if (timer.getHours() == 1 && ledFlag1) {
            brightArray[1] = brightArray[2];
            brightArray[2] = 0;
            ledIndex = 1;
            ledFlag1 = 0;
         } else if (timer.getHours() == 0 && ledFlag0) {
            brightArray[0] = brightArray[1];
            brightArray[1] = 0;
            ledIndex = 0;
            ledFlag0 = 0;
         }
    
        for (int i = 0; i < 4; i++){
          analogWrite(ledArray[i], brightArray[i]);
        }
        
        brightArray[ledIndex] += fade;
    
        if (brightArray[ledIndex] == 0 || brightArray[ledIndex] == 255) {
            fade = -fade;
        } 
        //  wait for 30 milliseconds to see the dimming effect; however, the delay() function appears to me screwing with the timer functionality
        //  delay(30);
      }
      dimFlag += 1;
         
		}
		
		
	}
	else  //button controls to start timer
	{

    // Cycle through options
    analogWrite(ledArray[index], 100);  // Illuminate new LED
    if (button1Pressed)
    {
      if (index == 3)
      {
        index = 0;
        start_hr = wait_array[index];
        start_mn = rest_array[index]; 
        analogWrite(ledArray[3], 0);        // Clear last LED
        analogWrite(ledArray[index], 100);  // Illuminate new LED
      }
      else
      {
        analogWrite(ledArray[index], 0);    // Clear last LED
        index += 1;
        start_hr = wait_array[index];
        start_mn = rest_array[index]; 
        analogWrite(ledArray[index], 100);  // Illuminate new LED      
      }

    }
    // Run option selected
    else if (button2Pressed)
    {
      timer.setTimer(start_hr, 0);
      //timer.setTimer(0, 10);      // For testing what the LED's do after the timer goes off
      
      timer.start();
      lock.close();
    }
    else
    {
      lock.open();
    }

	}
	

	 
}

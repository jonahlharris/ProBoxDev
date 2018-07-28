/*
 * Button.h
 *
 * Created: 2014-12-04 9:34:10 PM
 *  Author: amate_000
 */ 

#ifndef BUTTON_H_
#define BUTTON_H_


class Button {
	public:
		Button(int);

		bool isPressed();
		bool isHeld();
		
	private:
		unsigned int referencePin;
		bool triggered;

	
	};


#endif /* BUTTON_H_ */
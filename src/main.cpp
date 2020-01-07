#include <Arduino.h>
#include "jtag.h"

/* Use AVR USART */
//#define USART

#define ERROR -1
#define BAUD 9600

uint8_t data;

void control_LED(uint8_t command) {}

uint8_t state_pin29() {}

void setup() {

	Serial.begin(9600);

	pinMode(TMS, OUTPUT);
	pinMode(TCK, OUTPUT);
	pinMode(TDO, OUTPUT);
	pinMode(TDI, INPUT);
}

void loop() {

	if (Serial.available() > 0) {
			
			switch (Serial.read())
			{
				case 'd':
					Serial.print("ID CODE from PIC32: ");
					Serial.println(getIDCODE());
					break;
				case 1:
					control_LED(data);
					break;
				case 0:
					control_LED(data);
					break;
				case 'b':
					state_pin29();
					break;
				
				default:
					
					Serial.println("Default");
				break;

			}
		}
}
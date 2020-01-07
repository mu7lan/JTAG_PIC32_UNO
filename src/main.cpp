#include <Arduino.h>
extern "C" {
	#include "jtag.h"
	#include "sele.h"
}

#define ERROR -1
#define BAUD 9600

uint8_t data;

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
					Serial.println(getIDcode());
					break;
				case 1:
					ledState(LED_INPUT, HIGH);
					break;
				case 0:
					ledState(LED_INPUT, LOW);
					break;
				case 'b':
					Serial.print(buttonState() ? "Button is pressed!" : "Button is released!");
					break;
				
				default:
					Serial.println("Default");
				break;

			}
		}
}
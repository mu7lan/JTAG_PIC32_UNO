#include <Arduino.h>
#include "jtag.h"

void TCK_tick() {

	digitalWrite(TCK, HIGH);
	delay(5);
	digitalWrite(TCK, LOW);
	delay(5);
}

void IR() {
    sendBits(IR_TDO, IR_TMS, IR_SIZE);
}

void DR() {
    sendBits(DR_TDO, DR_TMS, DR_SIZE);
}

void sendBits(boolean tdo, boolean tms, uint8_t size) {

    for (int i = 0; i < size; i++) {
        digitalWrite(TMS, tms);
        digitalWrite(TDO, tdo);
        TCK_tick();
    }
    
}

void sendCommandInstruction(uint8_t cmd) {

    /* Instruction Register State */
    IR();
    sendBits(cmd, IR_UPDATE_TMS, IR_UPDATE_SIZE); 

    /* JTAG SHIFTS 1 after leaving shifthing state */
    sendBits(EXIT_TDO >> 1, EXIT_TMS, EXIT_SIZE - 1);
}

uint32_t readData(uint8_t size) {

    uint32_t value = 0;

    for (int i = 0; i < size; i++)
    {
        value |= digitalRead(TDI) << i;

        /* Force shift new position */
        sendBits(0, 0, 1);
    }

    return value;
    
}





#include <Arduino.h>
#include "jtag.h"
#include <util/delay.h>

void TCK_tick() {

	digitalWrite(TCK, HIGH);
	_delay_us(10);
	digitalWrite(TCK, LOW);
	_delay_us(10);
}

void Reset() {
    digitalWrite(TMS, HIGH);
    for (int i = 0; i < 5; i++)
    {
        TCK_tick();
    }
}

void IR() {
    Reset();
    SendBitsWithSize(IR_TDO, IR_TMS, IR_SIZE);
}

void DR() {
    SendBitsWithSize(DR_TDO, DR_TMS, DR_SIZE);
}

void sendBits(uint8_t tdo, uint8_t tms) {

    digitalWrite(TMS, tms);
    digitalWrite(TDO, tdo);
    TCK_tick();
    
}

void SendBitsWithSize(uint8_t tdo, uint8_t tms, uint8_t size) {

    for (int i = 0; i < size; i++)
    {
        sendBits(tdo & (1 << i), tms & (1 << i));
    }
    
}

void sendCommandInstruction(uint8_t cmd) {

    /* Instruction Register State */
    IR();

    SendBitsWithSize(cmd, IR_UPDATE_TMS, IR_UPDATE_SIZE);

    SendBitsWithSize(0, EXIT_TMS >> 1, EXIT_SIZE - 1);
    
}

uint32_t readData(uint8_t size) {

    uint32_t value = 0;

    for (int i = 0; i < size; i++)
    {
        value |= (uint32_t) digitalRead(TDI) << i;
        /* Force shift new position */
        sendBits(0, 0);
    }

    return value;
    
}





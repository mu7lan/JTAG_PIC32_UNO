#include <stdbool.h>
#include <stdint.h>

#define TMS 8
#define TCK 9
#define TDI 10 // PIC32 TDO
#define TDO 11 // PIC32 TDI
#define IDCODE_SIZE 32

#define IR_TMS  0b00110
#define IR_TDO  0b00000
#define IR_SIZE 5

#define DR_TMS  0b0010
#define DR_TDO  0b0000
#define DR_SIZE 4

#define EXIT_TMS  0b011
#define EXIT_TDO  0b000
#define EXIT_SIZE 3

#define IR_UPDATE_TMS   0b10000
#define IR_UPDATE_SIZE  5

/* Shifts out the device’s ID code. */
#define IDCODE 0b00001

/* Captures the I/O states of the component, providing a snapshot of its operation */
#define SAMPLEPRELOAD 0b00010

/* Allows the external circuitry and interconnections to be tested, 
by either forcing various test patterns on the output pins, or capturing test results from the input pins */
#define EXTERNALTEST 0b00110

#define LED_INPUT 18
#define LED_OUTPUT 19
#define LED_CONTROL 20
#define BOUNDARY 148
#define MCLR 129
#define BUTTON_REGISTER 3

void TCK_tick();
void IR();
void DR();
void sendBits(uint8_t tdo, uint8_t tms);
void SendBitsWithSize(uint8_t tdo, uint8_t tms, uint8_t size);
void sendCommandInstruction(uint8_t cmd);
uint32_t readData(uint8_t size);
#include <stdbool.h>
#include <stdint.h>

/* Required functions to match the unit course prject requirements */

/**
 * Get IDCODE
 * @return: IDCODE
 * */
uint32_t getIDcode();

/**
 * Changes Led State (ON/OFF)
 * */
void ledState(uint8_t position ,uint8_t how);

/**
 * Reads Button State
 * @return: true if button is pressed, false if not 
 * */
bool buttonState();
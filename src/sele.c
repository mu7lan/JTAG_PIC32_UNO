#include "sele.h"
#include "jtag.h"
#include <Arduino.h>

uint32_t getIDcode() {

    uint32_t received_data = 0;

    sendCommandInstruction(IDCODE);

    /*  Change to Data Register state in order to be able to read values in TDI */
    DR();
    received_data = readData(IDCODE_SIZE);

    sendBits(EXIT_TDO >> 1, EXIT_TMS, EXIT_SIZE - 1);

    return received_data;

}

void ledState(uint8_t position ,uint8_t how) {

    /* State of external test */
    sendCommandInstruction(EXTERNALTEST);
    
    /* Data Register State */
    DR();

    /* 18, 19 and 20 */
    for (int i = 0; i < BOUNDARY - 1; i++)
    {
        switch (i)
        {
        case LED_OUTPUT:
            sendBits(1, 0, how);
            break;
        case LED_CONTROL:
            sendBits(1, 0, 1);
            break;
        case MCLR:
            sendBits(1, 0, 1);
            break;
        
        default:
            sendBits(0, 0, 1);
            break;
        }
    }

    sendBits(EXIT_TDO, EXIT_TMS, EXIT_SIZE);
    
}

boolean buttonState() {

    uint8_t state = 0;

    sendCommandInstruction(SAMPLEPRELOAD);
    DR();

    /* Shift BUTTON_REGISTERS bits */
    for (int i = 0; i < BUTTON_REGISTER; i++)
        sendBits(0, 0, 1);

    state = digitalRead(TDI);

    sendBits(EXIT_TDO, EXIT_TMS, EXIT_SIZE);

    return state;

}
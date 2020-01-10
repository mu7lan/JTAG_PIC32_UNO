#include "sele.h"
#include "jtag.h"
#include <Arduino.h>

uint32_t getIDcode() {

    uint32_t received_data = 0;

    sendCommandInstruction(IDCODE);

    /*  Change to Data Register state in order to be able to read values in TDI */
    DR();
    received_data = readData(IDCODE_SIZE);

    SendBitsWithSize(0, EXIT_TMS, EXIT_SIZE);

    return received_data;

}

void ledState(uint8_t how) {

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
            sendBits(how, 0);
            break;
        case LED_CONTROL:
            sendBits(1, 0);
            break;
        case MCLR:
            sendBits(1, 0);
            break;
        
        default:
            sendBits(0, 0);
            break;
        }
    }

    /* Last shift from boundary scan */
    SendBitsWithSize(0, EXIT_TMS, EXIT_SIZE);
    
}

boolean buttonState() {

    uint8_t state = 0;

    sendCommandInstruction(SAMPLEPRELOAD);
    DR();

    /* Shift BUTTON_REGISTERS bits */
    for (int i = 0; i < BUTTON_REGISTER; i++)
        sendBits(0, 0);

    state = digitalRead(TDI);

    SendBitsWithSize(0, EXIT_TMS, EXIT_SIZE);

    return state;

}
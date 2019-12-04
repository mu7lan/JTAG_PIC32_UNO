#include <Arduino.h>

#define TMS 8
#define TCK 9
#define TDI 10 // PIC32 TDO
#define TDO 11 // PIC32 TDI
#define ERROR -1

#define FOSC 16000000 
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

uint8_t data;

void USART_Init(uint32_t ubrr){
	/*Set baud rate */
	UBRR0H = (uint8_t)(ubrr>>8);
	UBRR0L = (uint8_t)ubrr;
	/*Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (1<<UCSZ00)|(1<<UCSZ01);
}

void USART_Transmit(uint8_t data) {
    
    // put your code here, to send a data byte:
    /* Wait for empty transmit buffer */
    while (!(UCSR0A & (1<<UDRE0)));
    /* Put data into buffer, sends the data */ 
    UDR0 = data;
}

void USART_Transmit_String(const char *value)
{
	int k = 0;
	while( value[k] != '\0' )
	{
		/* Wait for empty transmit buffer */
		while ( !( UCSR0A & (1<<UDRE0)) );
		/* Put data into buffer, sends the data */
		UDR0 = value[k];
		k++;
	}
}

int USART_Receive(uint8_t *buffer) {
    
    // put your code here, to receive a data byte using multi processor communication mode:
	unsigned char status;

    /* Wait for data to be received */ 
    while (!(UCSR0A & (1<<RXC0)));

    /* Get status and 9th bit, then data */
    /* from buffer */ 
    status = UCSR0A; 

    /* If error, return -1 */
    if (status & ((1<<FE0)|(1<<DOR0)))
        return ERROR;// <- is signed

	*buffer = UDR0;
}

void TCK_tick () {
	digitalWrite(TCK, HIGH);
	_delay_us(1);
	digitalWrite(TCK, LOW);
}

uint32_t get_IDCODE() {}

void control_LED(uint8_t command) {}

uint8_t state_pin29() {}

void setup() {
	USART_Init(MYUBRR);

	pinMode(TMS, OUTPUT);
	pinMode(TCK, OUTPUT);
	pinMode(TDO, OUTPUT);
	pinMode(TDI, INPUT);
}

void loop() {
	
	if (USART_Receive(&data) != ERROR) {

		switch (data)
		{
			case 'd':
				get_IDCODE();
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
				/* print something */
				USART_Transmit_String("DEFAULT");
				break;

		}
	}
}
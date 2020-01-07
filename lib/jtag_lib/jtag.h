
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

/* Commands JTAG */
#define IDCODE 0b00001
#define SAMPLEPRELOAD 0b00010
#define EXTERNALTEST 0b00110

void TCK_tick();
void IR();
void DR();
void sendBits(boolean tdo, boolean tms, uint8_t size);
uint32_t getIDCODE();
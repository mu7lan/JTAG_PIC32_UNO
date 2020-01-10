# JTAG_PIC32_UNO
Control a ChipKIT Uno32 board just using its MCU JTAG port connected to an Arduino

## Contents

- [System Concept](#System-Concept)
- [Requirements](#Requirements)
- [JTAG & PIC32MX320F128H](#JTAG-&-PIC32MX320F128H)
    - [IEEE 1149.1 TAP controller](IEEE-1149.1-TAP-controller)
    - [Instruction Register](#Instruction-Register)
    - [Data Register](#Data-Register)
    - [Turn ON/OFF LED](#Turn-ON/OFF-LED)
    - [Read button state](#Read-button-state)

## System Concept

![JTAG_UNO](https://user-images.githubusercontent.com/38976366/72176750-f5518600-33d6-11ea-9c91-0354eb38676a.png)

## Requirements

- Get IDCODE value
- Turn ON and OFF ChipKIT Uno32 board LED
- Check Button state

## JTAG & PIC32MX320F128H

JTAG ports:
- TDI (Test Data In)
- TDO (Test Data Out)
- TCK (Test Clock)
- TMS (Test Mode Select)

#### IEEE 1149.1 TAP controller

To control the PIC32 using JTAG port it is necessary to understand the IEEE 1149.1 TAP controller state diagram:

<img width="424" alt="Captura de ecrã 2020-01-10, às 18 33 55" src="https://user-images.githubusercontent.com/38976366/72177334-5e85c900-33d8-11ea-9457-43b5eb784abb.png">

The first thing to do, is to *reset*. In other words, be sure that the TAP controller is on Test-Logic-Reset.

By now, it is possible to check that to enter in the Intruction Register Shifter (SHIFT-IR), it is necessary 5 TCK cycles and for each, the TMS should be: 0, 1, 1, 0 and 0. Stoping at Shift-IR.
For this case, IR_TMS can be defined as 0b00110.

#### Instruction Register

The instructions are defined by the microcontroller manufacturer:

````
 "extest (00110),"  & 
 "bypass (11111),"  & 
 "sample (00010),"  & 
 "preload (00010),"  & 
 "highz (00000),"  & 
 "idcode (00001),"  & 
 "swtap_chip (00100),"  & 
 "swtap (00101),"  & 
 "mchp_cmd (00111),"  & 
 "mchp_scan (01000)"  ; 
````

To write an instruction, it is necessary 5 TCK cycles since none is above 5 bits size and as this is an instruction, the Arduino should write through TDO into the PIC32 TDI. 
So, for instruction **idcode**, the TDO must be written like this: 1, 0, 0, 0 and 0. 

When the instruction state ends it changes into a **exit state** automatically. So from now on, the TAP controller is in Exit1-IR state. 
To return to Idle mode, it is needed 2 TCK cycles and the TMS should be written like this: 1 and 0. 

#### Data Register

To match the requirements of the project, it is necessary to use the boundary scan register (BSR).
When the instruction is SAMPLE or EXTEST, the BSR is placed between the TDI and TDO pins, like this:

<img width="691" alt="Captura de ecrã 2020-01-10, às 19 16 49" src="https://user-images.githubusercontent.com/38976366/72179863-c2f75700-33dd-11ea-9e1e-eff325409910.png">

Each I/O pin has an input cell, an output cell and a control cell, respectively to control the input, to control the output, and to enable the output.

Now, to enter Data Register Shift State, it is needed 4 TCK cycles (from reset, 3 from idle) and for each the TMS should be set like this: 0, 1, 0, and 0. In this case DR_TMS can be defined as: 0b0010.

To turn on the LED, first it is necessary to know which I/O pin is the LED and its Boundary Scan Cell(BSC) cells. 
From the ChipKIT Uno32 datasheet:
<img width="754" alt="Captura de ecrã 2020-01-10, às 19 33 48" src="https://user-images.githubusercontent.com/38976366/72180915-24202a00-33e0-11ea-952d-550390fcdadb.png">

The LED matches with RF0 signal and from the Boundary-Scan Description Language file, it is possible to know each cell position in the Boundary Scan:
````
"18  (    BC_4,      RF0,    input, X)," &    
"19  (    BC_1,      RF0,  output3, X, 20, 0, Z)," & 
"20  (    BC_2,        *,  control, 1)," & 
````
 Now it is possible to *write* in this I/O pin through JTAG.
 
 #### Turn ON/OFF LED
 
Since EXTEST instruction allows the external circuitry and interconnections to be tested, it is necessary to write it into the instruction register.

After that, and on Idle state, should enter Data Register column, stoping at Shift-DR state. Runnning all cell positions from 0 to 147. 

All cells should be written set to 0, but not the cells in position 19, 20 or 129, because 19 is the output cell from the LED, 20 is the output-enable and 129 is MCLR register.
The output cell at 19 should be set to 1 when it is supposed to be turned ON and set to 0 when it is supposed to be turned OFF. The output-enable, as the name says, should be set to 1 to enable the output. And finally, MCLR should be set to 1 because setting it to 0, will force an exit from Enhanced JTAG mode. 

Further it returns into idle state.

 #### Read button state
 
Since SAMPLE instruction captures the I/O states of the component, it is necessary to write it into the instruction register.
 
After that, and on idle state, should enter Data Register column, stoping at Shift-DR state. The state of the button is read after N TCK cycles, where N is the button I/O pin.

Further it returns into idle state.
 

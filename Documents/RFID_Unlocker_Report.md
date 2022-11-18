# RFID Unlocker
Charlie Dauchess  
MES Yellow Seahorses

**THIS IS A DRAFT**

## Application Description
This system provides a method to control access to a small electric vehicle through an RFID keytag.  The RFID keytag allows the system to identify if an authorized user is attempting to enable the vehicle.  An LED is used to indicate the system status to the user.  After a period of being stationary the system will alert the user by blinking the LED, and then subsequently disable the vehicle.



## Hardware Description

I'm using the Raspberry Pi PICO development board for this project.  This has a Raspberry Pi RP2040 microcontroller, a dual-core Arm Cortex M0+ processor. 

- RP2040
    - Dual core Arm Cortex M0+
- RFID: MFRC522 : SPI
    - borrowed arduino library from [here](https://github.com/miguelbalboa/rfid)
- Latching Relay : GPIO
- WS2812B LED
    - Fun with colors!
    - Borrowed PIO code from RP2040 examples
- 12V to 5V DC DC Converter
    - Accepting 12V external power
- [ADXL343](https://www.analog.com/media/en/technical-documentation/data-sheets/ADXL343.pdf) Accelerometer : I2C
    - This could prove challenging with just a 3 axis sensor.  This may be a point for future improvement
- Button for user interface : GPIO with interrupt
- CAT24C32 EEPROM : I2C

### PCB Schematic:
![pcb schematic](/Images/Final_PCB_Schematic.png)


## Software Description

This project is built on the RP2040 SDK.

Overall description goes here.

### ADXL343

### CAT24C32

### Relay
I used a latching relay for this project which requires different close and open functions.  This module contains functions to intialize the GPIO pins used for the relay, open, and close the relay.  This module also uses the alarm function from the RP2040 SDK to hold the specified GPIO pin high for only the short period of time that is required to switch the relay state.

### MFRC522
Borrowed arduino library from [here](https://github.com/miguelbalboa/rfid).  I did some modifications and removed unused functions to make the library compatible with the RP2040 SDK.  I also wrote an adapter layer (MFRC522_RP2040) to adapt the arduino functions to the RP2040 SDK.

ADD LICENSE INFORMATION

### Console
Borrowed from Elecia's demo.  (ADD LINK HERE)  

### Neopixel
Borrowed from RP2040 Examples (ADD LINK HERE)



## Diagrams

I need to update and improve these, but a start nonetheless.

![Hardware Diagram](/Images/HardwareDiagram.png)
![Software Diagram](/Images/SoftwareDiagram.png)
![Hierarchy of Control](/Images/HierarchyOfControl.png)
![State Table](/Images/StateTable.png)

## Build Instructions

Correct these to be a bit more descriptive, but a start here.

1. You have already installed OpenOCD with picoprobe, according to the steps in [Getting Started](https://datasheets.raspberrypi.org/pico/getting-started-with-pico.pdf), Appendix A
2. Your VSCode has already been configured according to the steps [here](https://shawnhymel.com/2096/how-to-set-up-raspberry-pi-pico-c-c-toolchain-on-windows-with-vs-code/)
3. You have installed the picoprobe uf2 to one Pico, and it is connected to the other Pico with the correct wiring (refer again to Appendix A).

## Power Analysis

This will be battery powered, albeit by a big battery, so power analysis would be useful to understand the impact on battery life will be however small it is.

## Future

Currently a placeholder.  Good place for reflections and lessons learned at the end of the project.

Time constraints led me to modify my initial plan of having a modifiable authorized user list.  In the future I plan on adding a command in the console that has a process for adding additonal users.  Also I'd like to improve my method of determining inactivity.  The current method may prove to be challenging to scale to larger timeout times due to the large amount of memory required.  I may have to periodically average the entire sample set to reduce the required memory.

## Grading

Currently a placeholder

## To Do Items
This is likely to be an everchanging list
- [X] RFID Driver
- [X] LED Driver
- [X] Button Driver
- [X] Relay Output Driver
- [X] EEPROM Driver
- [X] Accelerometer Driver
- [X] Bring everything together!
- [X] Bonus: Custom PCB
    - making progress here
    ![pcb schematic](/Images/Final_PCB_Schematic.png)


## Minimum Viable Project
- [X] Read an RFID Keytag
- [X] Compare RFID ID to authorized user database
- [X] Close a relay to enable the vehicle
- [ ] Have a method to add and remove users from the database - This is going to be added later.
- [X] Use an LED to indicate status of the system
- [X] Have a method to detect a stationary vehicle for a timeout

 I have all of the hardware intended to be used in breakout board form.  A custom PCB is currently in progress, but not neccesary to demonstrate complete function.  It mostly cleans up the spaghetti of bread board wiring. 

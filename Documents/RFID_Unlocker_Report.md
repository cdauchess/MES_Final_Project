# RFID Unlocker
Charlie Dauchess  
MES Yellow Seahorses

**THIS IS A DRAFT**

## Application Description
This system provides a method to control access to a small electric vehicle through an RFID keytag.  The RFID keytag allows the system to identify if an authorized user is attempting to enable the vehicle.  An LED is used to indicate the system status to the user.  After a period of being stationary the system will alert the user by blinking the LED, and then subsequently disable the vehicle.



## Hardware Description

I'm using the Raspberry Pi PICO development board for this project.  This has a Raspberry Pi RP2040 microcontroller, a dual-core Arm Cortex M0+ processor. 

- RP2040
- RFID: MFRC522
- Latching Relay
- WS2812B LED
    - Fun with colors!
- 12V to 5V DC DC Converter
    - Accepting 12V external power
- ADXL343 Accelerometer
    - This could prove challenging with just a 3 axis sensor.  This may be a point for future improvement
- Button for user interface
- CAT24C32 EEPROM

## Software Description

This project is built on the RP2040 SDK.

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

## Grading

Currently a placeholder

## To Do Items
This is likely to be an everchanging list
- [ ] RFID Driver
- [ ] Many things to list here
- [ ] Bonus: Custom PCB
    - making progress here
    ![pcb schematic](/Images/CurrentPCBSchematic.png)


## Minimum Viable Project
- [ ] Read an RFID Keytag
- [ ] Compare RFID ID to authorized user database
- [ ] Close a relay to enable the vehicle
- [ ] Have a method to add and remove users from the database
- [ ] Use an LED to indicate status of the system
- [ ] Have a method to detect a stationary vehicle for a timeout

 I have all of the hardware intended to be used in breakout board form.  A custom PCB is currently in progress, but not neccesary to demonstrate complete function.  It mostly cleans up the spaghetti of bread board wiring. 
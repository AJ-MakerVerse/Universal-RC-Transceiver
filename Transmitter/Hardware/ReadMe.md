**Transmitter Hardware**  
This folder contains the physical assembly details for the Universal RC Transmitter. The build is designed to be soldered onto a standard Universal PCB for a compact and durable finish.

**Documentation Files**
* Wiring Diagram: The complete schematic and point-to-point wiring guide.  
* Bill of Materials (BoM): A full list of components required for the build.

**Pin Mapping Table**  
Use the follwoing list to wire your components to the Arduino Nano:  

**Digital Inputs (Switches & Buttons)**

* DPDT_1 =  D6	
* DPDT_2 =	D7	
* DPDT_3 =	D0 (RX)	
* DPDT_4 =	D1 (TX)	
* PB_1   =	D5	
* PB_2   =	D2	
* Tare   =  D8

**Analog Inputs (Joysticks \& Pots)**

* JS_1_VRy = A0	
* JS_1_VRx = A1	
* JS_1_SW  = D4	
* JS_2_VRy =	A6	
* JS_2_VRx =	A7	
* JS_2_SW  =	D3	
* Pot_1    =	A2	
* Pot_2		 =  A3

**Communication Modules**  
* nRF24L01 Radio 	\[I2C] -	D9 (CE), D10 (CSN), D11 (MOSI), D12(MISO), D13(SCK)
* MPU-6500 IMU	\[SPI] -	A4 (SDA), A5 (SCL)

**Assembly Notes**

Universal PCB Layout:

* Placement: Place the Arduino Nano in the center using female header pins. This allows you to replace the Nano if it ever burns out.
* Power: The nRF24L01 is very sensitive to power noise. It is highly recommended to solder a 10µF to 100µF capacitor across the VCC and GND pins of the radio module.
* Pull-ups: The firmware uses internal INPUT\_PULLUP. Ensure all switches and buttons are wired to GND when closed.



IMU Orientation:  
Mount the MPU-6500 flat on the PCB. If your "Forward" tilt is giving you "Roll" data instead of "Pitch," check the orientation of the module or adjust the axes in the firmware.


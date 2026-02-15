**Receiver Firmware**

This folder contains the reference code for the Arduino Nano receiver. Its primary purpose is to intercept the data packets from the Universal Transmitter and output the values to the Serial Monitor for debugging and calibration.

**Required Libraries**  
To compile this code, you must install the following library:

* RF24 (by TMRh20): For nRF24L01 radio communication.

**Logic Overview**  
The receiver acts as a passive listener. It is configured to match the Transmitter's settings exactly:

* Address: "00009"
* Channel: 105
* Data Rate: 250 kbps
* CRC: 16-bit

**The "Packed" Struct**  
The most critical part of this firmware is the Values struct. Because the Transmitter uses \_\_attribute\_\_((packed)), the Receiver must use the exact same definition to ensure the bytes align correctly in the Arduino's memory.

**Using the Serial Monitor**  
Once the code is flashed, open the Serial Monitor in the Arduino IDE to see the live data stream.

Settings:

* Baud Rate: 115200
* Format: The output will display PB (Push Buttons), DPDT (Switches), POT (Potentiometers), JS (Joysticks), and IMU (Pitch/Roll) values.

Reading the Output

* Digital Pins: Values are shown as 0 (Released) or 1 (Pressed).
* Analog Pins: Values range from 0 to 1023.
* IMU: Pitch and Roll are displayed in degrees. Note that the code divides the incoming value by 10 (e.g., a transmitted 450 becomes 45.0°).

**Troubleshooting the Link**  
If you don't see data in the Serial Monitor:

* Radio Check: On startup, the code checks if the nRF24L01 is connected. If you see "Radio Hardware Not Found!", check your SPI wiring.
* Address Mismatch: Ensure the Address variable in both TX and RX code is exactly "00009".
* Power Issues: The nRF24L01 on a breadboard is prone to electrical noise. If the data is "choppy" or stops randomly, add a capacitor (10µF+) across the radio's VCC and GND.

**Transmitter Firmware**

This folder contains the source code for the Universal RC Transmitter. The firmware is optimized for high-speed transmission (100Hz) and features advanced IMU data processing for motion control.

**Required Libraries**  
To compile this code, you must install the following libraries in the Arduino IDE:

* RF24 (by TMRh20): For nRF24L01 radio communication.
* FastIMU (by Tutto): For high-speed MPU-6500 data processing.
* SPI \& Wire: (Built-in Arduino libraries).

**Radio Configuration**  
The transmitter is configured for maximum reliability and low latency:

* Channel: 105 (2.505 GHz)
* Data Rate: 250 kbps (Chosen for better range and signal penetration).
* Payload: Dynamic payloads enabled with 16-bit CRC error detection.
* Power Level: Currently set to RF24\_PA\_MIN for bench testing.

Note: For field use, uncomment radio.setPALevel(RF24\_PA\_MAX); in the setup() function.

**Motion Control (IMU)**  
The firmware uses an MPU-6500 to calculate Pitch and Roll.

* Tare System: Pin D8 acts as a "Deadman Switch."
* The IMU only streams data when D8 is held down.
* On press, it "Tares" (zeros) the current orientation, allowing you to use the remote in any comfortable position.
* Complementary Filter: We use a filter coefficient of 0.96 to combine Accelerometer and Gyroscope data, providing smooth angles without "drift."

**Data Efficiency (Bit-Packing)**  
To keep the latency low, we pack all inputs into a custom C++ struct. This allows us to send 14 different inputs in a single 17-byte packet.

**Troubleshooting (Blink Codes)**  
If the transmitter fails to start, the Built-in LED (Pin 13) will blink a specific pattern:

* Single Blink: nRF24L01 Radio Hardware not found. Check SPI wiring and power.
* Double Blink: MPU-6500 IMU not found. Check I2C (A4/A5) connections.

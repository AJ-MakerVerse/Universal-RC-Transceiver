**Universal RC Transceiver**



This project is a high-performance, DIY Radio Control Transmitter designed to be a "Master Remote" for future robotics projects. While the transmitter is a complete hardware build (Universal PCB + 3D Printed Case), the receiver is provided as a reference prototype to jumpstart integration into any Arduino-based vehicle.



**The Vision**

The goal was to create a single, reliable handheld controller with a standardized data packet. Whether you are building a drone, a rover, or a robotic arm, you simply drop the provided Receiver code into your project and start mapping controls.



**Features**

Hardware Capabilities:

* Multi-Input Control: Supports 2 Joysticks (4-axis), 2 Potentiometers, 4 DPDT Switches, and 2 Push Buttons.
* Motion Control: Integrated MPU-6500 IMU allows for tilt-based steering (Pitch/Roll) with a dedicated "Tare" deadman switch.
* Universal Build: Designed for a standard Universal PCB (Perfboard), making it accessible for DIY builders.
* Ergonomic Design: Custom 3D-printable housing for a professional handheld feel.



Firmware Intelligence:

* High-Speed Link: Operates at 100Hz (10ms refresh rate) for low-latency control.
* Optimized Payload: Uses a Packed C++ Struct (17 bytes) to fit all data into a single nRF24L01 packet (under the 32-byte limit).
* Bit-Packing: Compresses 8 digital inputs into a single byte to save bandwidth and improve reliability.
* Error Handling: Built-in LED blink codes for hardware troubleshooting (Radio or IMU disconnected).
* Robust Protocol: Uses 16-bit CRC error detection, Auto-Acknowledgment, and a specific 250kbps data rate for maximum range and penetration.



**Project Structure**

This repository is organized into two main modules:



**1. Transmitter (TX)**

The control hub of the system.

* Firmware: Source code and library dependencies.
* Hardware: Universal PCB layout and wiring diagrams.
* STLs: 3D Printable files for the transmitter case.



**2. Receiver (RX)**

The vehicle/device-side module.

* Firmware: Reference code to parse data and print to Serial Monitor.
* Hardware: Breadboard wiring guide for prototyping.



Each sub-folder contains its own dedicated README with specific assembly and setup instructions.



**Quick Start**

* Build the Hardware: Start with the Transmitter Hardware Guide.
* Library: Install RF24 and FastIMU libraries in your Arduino IDE.
* Flash the Code: Upload the TX Firmware and RX Firmware.
* Test: Power on both units and verify the link.



**License**

This project is licensed under the MIT License.



**Contributing**

Feel free to fork this project, report bugs via Issues, or submit Pull Requests to improve the protocol or hardware designs!



**To see more of my projects, feel free to connect with me on LinkedIn - https://www.linkedin.com/in/abhayjain-makerverse**



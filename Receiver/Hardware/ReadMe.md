**Receiver Hardware**

This folder contains the wiring instructions for the receiver prototype. Currently, the receiver is designed for a breadboard setup to allow for quick testing and easy integration into different projects.

**Documentation Files**	

* Breadboard Diagram: Visual guide for jumper wire placement.
* Bill of Materials (BoM): A full list of components required for the build.

**Pin Mapping (Arduino Nano)**  
Since the receiver only requires the nRF24L01 module, the wiring is straightforward:

* VCC  = 3.3V
* GND  = GND	
* CE   = D9	 
* CSN  = D10	 
* SCK  = D13	
* MOSI = D11	
* MISO = D12

**Pro Tip:** Breadboards are notorious for "noisy" power lines. If the receiver is struggling to stay connected, plug a 10µF-100µF capacitor directly into the breadboard across the nRF24L01's VCC and GND pins.

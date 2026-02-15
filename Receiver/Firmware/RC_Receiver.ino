//SPI Communication Library
#include <SPI.h>                                      //D11 = MOSI ; D12 = MISO ; D13 = SCK

//nRF24L01 Library
#include <nRF24L01.h>
#include <RF24.h>

//CE, CSN Pins
RF24 radio(9, 10); 

//Transmitter Address
const byte Address[6] = "00009";                      //Must be same as Transmitter Address

//Receiver Data Type: Must be same as Transmitter
struct __attribute__((packed)) Values{
  uint8_t Digital;
  uint16_t Analog[6];
  int16_t Euler[2];
}; 

//Receiver Object
Values Data;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);                               //Initialize Serial Monitor

  if (!radio.begin()) {
    Serial.println("Radio Hardware Not Found!");
    while (1);
  }                                                   //Safety Check

  radio.enableDynamicPayloads();                      //Allow Packets of different Byte sizes
  radio.setAutoAck(true);                             //Enables Auto-Acknowledgment
  radio.setCRCLength(RF24_CRC_16);                    //Uses a 16-bit checksum for better error detection

  radio.setChannel(105);                              //Channel 0-125 :: 2.4 - 2.525 GHz
  radio.setDataRate(RF24_250KBPS);                    //Sets Transmission Speed to 250 kbps (250 kbps - 2 Mbps)

  radio.setPALevel(RF24_PA_MIN);                      //Sets Power Amplifier (PA) to MINIMUM, Range: 1 - 2 Meters
  //radio.setPALevel(RF24_PA_LOW);                      //Range: 5 - 15 Meters
  //radio.setPALevel(RF24_PA_HIGH);                     //Range: 15 - 30 Meters
  //radio.setPALevel(RF24_PA_MAX);                      //Range: 30 - 70+ Meters

  radio.openReadingPipe(1, Address);                     //Sets the destination address for transmission
  radio.startListening();                             //Puts radio into Receiver (RX) Mode

  Serial.println("Receiver Initialized. Waiting for data...");
}

void loop() {
  // put your main code here, to run repeatedly:

  if (radio.available()){
    radio.read(&Data, sizeof(Data));                  //Reading the Struct

    // 1. Push Buttons (Pins D2 and D5)
    Serial.print("PB_1: ");       Serial.print((Data.Digital >> 5) & 0x01);
    Serial.print("  PB_2: ");     Serial.println((Data.Digital >> 2) & 0x01);

    // 2. DPDT Switches (Pins D0, D1, D6, D7)
    Serial.print("DPDT_1: ");     Serial.print((Data.Digital >> 6) & 0x01);
    Serial.print("  DPDT_2: ");   Serial.print((Data.Digital >> 7) & 0x01);
    Serial.print("  DPDT_3: ");   Serial.print((Data.Digital >> 0) & 0x01);
    Serial.print("  DPDT_4: ");   Serial.println((Data.Digital >> 1) & 0x01);

    //3. Potetiometers (Pins A2 and A3)
    Serial.print("POT_1: ");      Serial.print(Data.Analog[2]);
    Serial.print("  POT_2: ");    Serial.println(Data.Analog[3]);

    //4. Joysticks (Pins A0, A1, A6, A7, D3 and D4)
    Serial.print("JS_1_VRx: ");   Serial.print(Data.Analog[1]);
    Serial.print("  JS_1_VRy: "); Serial.print(Data.Analog[0]);
    Serial.print("  JS_1_SW: ");  Serial.print((Data.Digital >> 4) & 0x01);

    Serial.print("  JS_2_VRx: "); Serial.print(Data.Analog[7]);
    Serial.print("  JS_2_VRy: "); Serial.print(Data.Analog[6]);
    Serial.print("  JS_2_SW: ");  Serial.println((Data.Digital >> 3) & 0x01);

    //5. IMU Data (Pitch and Roll)
    Serial.print("Pitch: ");      Serial.print((Data.Euler[1]) / 10);
    Serial.print("  Roll: ");     Serial.println((Data.Euler[0] / 10));

    Serial.println();
    
  } 
}

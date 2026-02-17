/*
Arduino Nano Pinout
  Digital Input:    |   Analog Input:    |    SPI Communication Pins:   |   I2C Communication Pins:

  RX0 = DPDT_4      |   A0 = JS_1_VRy    |    D9  = CE                  |   A4 = SDA
  TX1 = DPDT_3      |   A1 = JS_1_VRx    |    D10 = CSN                 |   A5 = SCL
  D2  = PB_2        |   A2 = POT_1       |    D11 = MOSI                |
  D3  = JS_2_SW     |   A3 = POT_2       |    D12 = MISO                |
  D4  = JS_1_SW     |   A6 = JS_2_VRy    |    D13 = SCK                 |
  D5  = PB_1        |   A7 = JS_2_VRx    |                              |
  D6  = DPDT_1      |                    |                              |
  D7  = DPDT_2      |                    |                              |
  D8  = Tare/PB_3   |                    |                              |
*/

//SPI Communication Library
#include <SPI.h>                                      //D11 = MOSI ; D12 = MISO ; D13 = SCK

//nRF24L01 Library
#include <nRF24L01.h>
#include <RF24.h>

//I2C Communication Library
#include <Wire.h>                                     //A4 = SDA ; A5 = SCL

//MPU-6500 Library
#include <FastIMU.h>

//CE, CSN Pins
RF24 radio(9, 10); 

//Transmitter Address
const byte Address[6] = "00009";                      //Must be same as Receiver Address

//MPU-6500 I2C Address
const uint8_t IMU_ADDRESS = 0x68;                     //IMU: Inertial Measurement Unit Sensor
const uint8_t Tare = 8;                               //IMU Deadman Switch

//MPU-6500 Object
MPU6500 IMU;

//IMU Data Structure
calData Calib;                                        //Calibration Data
AccelData Accel;                                      //Accelerometer Data
GyroData Gyro;                                        //Gyroscope Data

//Pitch and Roll Calculation Variables
float OAx, OAy, OAz, OGx, OGy;                        //Accel & Gyro Offset values 
float roll, pitch, sRoll, sPitch;                     //s : snapshot values
float alpha = 0.96;                                   //Complementary Filter
unsigned long time;
bool running = false;                                 //IMU Tracking Status

//Transmitter Data Type
struct __attribute__((packed)) Values{
  uint8_t Digital;                                    //8 digital inputs packed into 8 bits (Bit-Packing)     :1 Bytes
  uint16_t Analog[6];                                 //6 analog inputs (0 - 1023)                            :12 Bytes
  int16_t Euler[2];                                   //Euler Angles (Pitch and roll)                         :4 Bytes
};                                                    //Total                                                 :17 Bytes < 32 Bytes

//Transmitter Object
Values Data;

//Analog Input Pin Array
uint8_t analogPins[] = {A0, A1, A2, A3, A6, A7};

//Accel & Gyro Value Calibration Function
void calibrate(bool GyroOnly){
  float Ax=0, Ay=0, Az=0, Gx=0, Gy=0;
  for(int i=0; i<100; i++){
    IMU.update();                                     //Update IMU readings
    IMU.getAccel(&Accel); IMU.getGyro(&Gyro);         //Getting current IMU Data
    //Sum of 100 Values
    Ax+=Accel.accelX; Ay+=Accel.accelY; Az+=Accel.accelZ;     
    Gx+=Gyro.gyroX; Gy+=Gyro.gyroY;
    delay(5);
  }
  if(!GyroOnly){ 
    OAx=Ax/100; OAy=Ay/100; OAz=(Az/100)-1.0;         //Calculating Accel Offset Values as avg of 100 values
  }
  OGx=Gx/100; OGy=Gy/100;                             //Calculating Gyro Offset Values as avg of 100 values
}

//Euler Angles Calculation Function
void Angles(bool Track){
  IMU.update();                                       //Update IMU readings
  IMU.getAccel(&Accel); IMU.getGyro(&Gyro);           //Getting current IMU Data

  //Subtract the offset values from current values to get the data close to 0
  float Ax = Accel.accelX-OAx, Ay = Accel.accelY-OAy, Az = Accel.accelZ-OAz;    
  float Gx = Gyro.gyroX-OGx, Gy = Gyro.gyroY-OGy;

  float dt = (millis()-time)/1000.0; time = millis(); //Calculating Instantaneous time
  
  //Calculating Roll and Pitch based on Accel Values
  float rollAccel = atan2(Ay, Az) * 180.0/PI;
  float pitchAccel = atan2(-Ax, sqrt(Ay*Ay + Az*Az)) * 180.0/PI;

  if (Track){                                         //Jump directly to physical angle on first frame
    roll = rollAccel;
    pitch = pitchAccel;
  }
  else{                                               //Use normal filter during streaming
    roll = alpha * (roll + Gx * dt) + (1.0-alpha) * rollAccel;
    pitch = alpha * (pitch + Gy * dt) + (1.0-alpha) * pitchAccel;
  }
}

void setup(){
  //put your code here, to run once:

  Wire.begin();                                       //Initialize I2C Communication
  Wire.setClock(400000);                              //Set I2C to 400kHz for faster IMU updates

  //Initializing nRF24L01 Module
  if (!radio.begin()){                                //Initializes Radio Chip
    pinMode(13, OUTPUT);                              //Explicitly take control for error signal
    while (1){
      digitalWrite(13, HIGH); delay(1000);
      digitalWrite(13, LOW);  delay(2000);
      //Built-In LED - Single Blink : Radio is disconnected
    } 
  }                                                   //If radio fails, stop here (Safety Check)                                 

  radio.enableDynamicPayloads();                      //Allow Packets of different Byte sizes
  radio.setAutoAck(true);                             //Enables Auto-Acknowledgment
  radio.setCRCLength(RF24_CRC_16);                    //Uses a 16-bit checksum for better error detection

  radio.setChannel(105);                              //Channel 0-125 :: 2.4 - 2.525 GHz
  radio.setDataRate(RF24_250KBPS);                    //Sets Transmission Speed to 250 kbps (250 kbps - 2 Mbps)

  radio.setPALevel(RF24_PA_MIN);                      //Sets Power Amplifier (PA) to MINIMUM, Range: 1 - 2 Meters
  //radio.setPALevel(RF24_PA_LOW);                      //Range: 5 - 15 Meters
  //radio.setPALevel(RF24_PA_HIGH);                     //Range: 15 - 30 Meters
  //radio.setPALevel(RF24_PA_MAX);                      //Range: 30 - 70+ Meters

  radio.setRetries(5, 15);                            //1250us delay, 15 retries

  radio.openWritingPipe(Address);                     //Sets the destination address for transmission
  radio.stopListening();                              //Puts radio into Transmitter (TX) Mode

  //Initializing MPU-6500
  if(IMU.init(Calib, IMU_ADDRESS) !=0){               //Initializes IMU Chip
    pinMode(13, OUTPUT);                              //Explicitly take control for error signal
    while (1){
      digitalWrite(13, HIGH); delay(1000);
      digitalWrite(13, LOW);  delay(500);
      digitalWrite(13, HIGH); delay(1000);
      digitalWrite(13, LOW); delay(2000);
      //Built-In LED - Double Blink : IMU is disconnected
    }
  }                                                   //If IMU fails, stop here (Safety Check)

  //Setting Arduino digital pins as INPUT
  for(int i = 0; i <= 8; i++){
    pinMode(i, INPUT_PULLUP);
  }

  calibrate(0);                                       //Calibrating both Accel & Gyro

}

void loop(){
  //put your code here, to run repeatedly:

  static unsigned long lastSend = 0;
  if (millis() - lastSend < 10) return; 
  lastSend = millis();                                //Send data every 10ms (100Hz)

  //Store Roll and Pitch
  bool pressed = (digitalRead(Tare) == LOW);

  if (!running && pressed){                           //IMU is not tracking but the Button is pressed
    calibrate(1);                                     //Calibrating only Gyro to remove Gyro drift
    time = millis();                                  //Reset timer
    Angles(true);                                     //Force filter to match current tilt exactly
    sRoll = roll; sPitch = pitch;                     // Snapshot this as "Zero"
    running = true;                    
  } 
  else if (running){                                  //IMU is tracking
    if (!pressed){                                    //Button is released
      running = false; 
      Data.Euler[0] = 0;  Data.Euler[1] = 0;
    }else{
      Angles(false);                                  //Run normal filter
      Data.Euler[0] = (int16_t)((roll - sRoll) * 10);
      Data.Euler[1] = (int16_t)((pitch - sPitch) * 10);
    }
  }
  else{
    Data.Euler[0] = 0;  Data.Euler[1] = 0;
  }

  //Store Analog Input
  for (int i = 0; i < 6; i++){
    Data.Analog[i] = analogRead(analogPins[i]);
  }

  //Store Digital Input
  Data.Digital = 0;                                   //Clears the Bucket : 0000 0000
  //Data.Digital = ~(PIND);
  for (int i = 0; i <= 7; i++){
    if (digitalRead(i) == LOW){                       //Button pressed
      Data.Digital |= (1 << i);                       //Stores value at the 'i' bit
    }
  }

  radio.write(&Data, sizeof(Data));                   //Transmits the Data
  
}

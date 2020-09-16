
// This code runs on ESP32. It is able to capture the Tx data from both the
// esp8266 and the STM 32 on the Shelly Dimmer 2. These data can be then used
// to analyse the communication protocole between the two MCU.

// Array to save the serial data
#define ARRAY_SIZE 10000
unsigned long serial1Time[ARRAY_SIZE];
byte serial1Data[ARRAY_SIZE];
int serial1Idx = 0;
unsigned long serial2Time[ARRAY_SIZE];
byte serial2Data[ARRAY_SIZE];
int serial2Idx = 0;

byte serialData[3] = {0x00, 0x00, 0x00};

void setup() {
  // For the Arduino
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }


  // The Rx/Tx pins of the Shelly dimmer should be connected to the pin 21 and 16 on the ESP32
  // To get the data from the MCU
  Serial1.begin(115600, SERIAL_8N1, 21, 20);  // Rx/Tx
  // To get the data from the esp8266
  Serial2.begin(115600, SERIAL_8N1, 16, 17);  // Rx/Tx


  printMenu();
}

void printMenu()
{
  Serial.println("M0: Init variables");
  Serial.println("M1: Show data of serial 1");
  Serial.println("M2: Show data of serial 2");
}

void initVariables()
{
  Serial.println("Initialising variables...");
  memset(serial1Time, 0x00, sizeof(serial1Time));
  memset(serial1Data, 0x00, sizeof(serial1Data));
  serial1Idx = 0;
  memset(serial2Time, 0x00, sizeof(serial2Time));
  memset(serial2Data, 0x00, sizeof(serial2Data));
  serial2Idx = 0;
  Serial.println("Done initialising variables...");
}

void printSerialData(int serialID)
{
  Serial.printf("Start printing data for serial %d\n", serialID);
  if (serialID == 1)
  {
    for (int i = 0; i < serial1Idx; i++)
    {
      Serial.printf("1 %d %lu 0x%02X\n", i, serial1Time[i], serial1Data[i]);
    }
  }
  else
  {
    for (int i = 0; i < serial2Idx; i++)
    {
      Serial.printf("2 %d %lu 0x%02X\n", i, serial2Time[i], serial2Data[i]);
    }
  }
  Serial.printf("Finished printing data for serial %d\n", serialID);
}

void loop()
{
  byte b;

  if (Serial.available())
  {
    serialData[0] = serialData[1];
    serialData[1] = serialData[2];
    serialData[2] = Serial.read();

    if (serialData[0] == 'M' && serialData[2] == '\n')
    {
      if (serialData[1] == '0')
        initVariables();
      else if (serialData[1] == '1')
        printSerialData(1);
      else if (serialData[1] == '2')
        printSerialData(2);
      printMenu();
    }
  }

  if (Serial1.available())
  {
    Serial1.readBytes(&b, 1);
    if (serial1Idx + 1 < ARRAY_SIZE)
    {
      serial1Time[serial1Idx] = micros();
      serial1Data[serial1Idx] = b;
      serial1Idx++;
    }
    else
      Serial.println("Overflow for serial 1");
  }
  if (Serial2.available())
  {
    Serial2.readBytes(&b, 1);
    if (serial2Idx + 1 < ARRAY_SIZE)
    {
      serial2Time[serial2Idx] = micros();
      serial2Data[serial2Idx] = b;
      serial2Idx++;
    }
    else
      Serial.println("Overflow for serial 2");
  }

}

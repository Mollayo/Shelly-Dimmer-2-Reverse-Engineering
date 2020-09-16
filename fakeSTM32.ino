// This is to emulate the STM MCU inside the Shelly dimmer 2
// The code works on ESP32. The ESP32 running this code is
// connected to a generic esp8266 on which the stock firmware
// of the Shelly dimmer 2 has been installed. Then, it is possible
// to access to the shelly web interface, modify the parameters and
// see what kind of data is sent to the STM MCU.

void setup() {
  // For the Arduino
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }



  // To get the data from the esp8266
  // The Rx/TX of the esp6266 should be connected to pin 17 and 16 of the ESP32
  Serial1.begin(115600, SERIAL_8N1, 16, 17);  // Rx/Tx
  // Set the default value of the programming mode

}

uint8_t _packet_counter = 0;
uint8_t _packet_start_marker = 0x01;
uint8_t _packet_end_marker = 0x04;

const uint8_t buffer_size = 255;
uint8_t tx_buffer[buffer_size];
uint8_t rx_buffer[buffer_size];
uint8_t rx_idx = 0;
uint8_t rx_payload_size = 0;
uint8_t rx_payload_cmd = 0;
const uint8_t rx_max_payload_size = 255;
uint8_t rx_payload[rx_max_payload_size];

const uint8_t CMD_SET_BRIGHTNESS = 0x02;
const uint8_t CMD_SET_BRIGHTNESS_ADVANCED = 0x03;
const uint8_t CMD_GET_STATE = 0x10;
const uint8_t CMD_GET_VERSION = 0x01;
const uint8_t CMD_SET_DIMMING_PARAMETERS = 0x20;
const uint8_t CMD_SET_DIMMING_TYPE_2 = 0x30;
const uint8_t CMD_SET_DIMMING_TYPE_3 = 0x31;
const uint8_t CMD_SET_WARM_UP_TIME = 0x21;

#define LEADING_EDGE 0x01
#define TRAILING_EDGE 0x02


const char* hexToStr(const uint8_t *s, uint8_t len)
{
  static char output[1000];
  if (len * 3 + 1 > sizeof(output))
  {
    sprintf(output, "buffer overflow in hexToStr");
    return output;
  }
  char *ptr = &output[0];
  int i;
  for (i = 0; i < len - 1; i++) {
    ptr += sprintf(ptr, "%02X_", s[i]);
  }
  if (i < len)
    ptr += sprintf(ptr, "%02X", s[i]);
  return output;
}

void sendCommand(uint8_t cmd, uint8_t *payload, uint8_t len) {
  uint8_t b = 0;

  delay(10);
  
  tx_buffer[b++] = _packet_start_marker;
  tx_buffer[b++] = _packet_counter;
  tx_buffer[b++] = cmd;
  tx_buffer[b++] = len;

  if (payload) {
    memcpy(tx_buffer + b, payload, len);
  }
  b += len;

  uint16_t c = crc(tx_buffer, b);

  tx_buffer[b++] = c >> 8; // crc first byte (big/network endian)
  tx_buffer[b++] = c; // crc second byte (big/network endian)
  tx_buffer[b] = _packet_end_marker;

  b++;

  Serial1.write(tx_buffer, b);
  //Serial.printf("dimmer: send packet %s\n", hexToStr(tx_buffer, b));

  _packet_counter++;
}

void processReceivedPacket(uint8_t payload_cmd, uint8_t* payload, uint8_t payload_size)
{
  // Command for getting the version of the STM firmware
  if (payload_cmd == CMD_GET_VERSION)
  {
    Serial.printf("dimmer: received packet %s\n", hexToStr(payload,payload_size));
    Serial.printf("Get version\n");
    // Send the packet
    uint8_t payload[]={0x35,0x02};
    sendCommand(payload_cmd, payload, sizeof(payload));
  }
  // Command for getting the state (brigthness level, wattage, etc)
  else if (payload_cmd == CMD_GET_STATE)
  {
    //Serial.printf("dimmer: received packet %s\n", hexToStr(payload,payload_size));
    //Serial.printf("Get state\n");
    /*
     * Some example of frames
       Power 116 W
      at 75% -> 00 00 EE 02 00 00 78 09 00 00 00 80 00 00 00 00 -> 2424   -> Shelly firmware: 121 W
      at 50% -> 00 00 F4 01 00 00 E1 05 00 00 00 80 00 00 00 00 -> 1505   -> Shelly firmware: 77 W
      at 25% -> 00 00 FA 00 00 00 91 01 00 00 00 80 00 00 00 00 -> 401    -> Shelly firmware: 21 W

      Power 46 w
      at 75% -> 00 00 EE 02 00 00 C5 03 00 00 00 80 00 00 00 00 -> 965
      at 50% -> 00 00 F4 01 00 00 66 02 00 00 00 80 00 00 00 00 -> 614
      at 25% -> 00 00 FA 00 00 00 AB 00 00 00 00 80 00 00 00 00 -> 171

    */
    uint8_t payload[]={0x00,0x00,0xEE,0x02,0x00,0x00,0x78,0x09,0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00};
    sendCommand(payload_cmd, payload, sizeof(payload));
  }
  else if (payload_cmd == CMD_SET_BRIGHTNESS)
  {
    Serial.printf("dimmer: received packet %s\n", hexToStr(payload,payload_size));
    Serial.printf("Set brightness\n");
    uint8_t payload[]={0x01};
    sendCommand(payload_cmd, payload, sizeof(payload));
  }
  else if (payload_cmd == CMD_SET_BRIGHTNESS_ADVANCED)
  {
    Serial.printf("dimmer: received packet %s\n", hexToStr(payload,payload_size));
    Serial.printf("Set brightness advanced\n");
    uint8_t payload[]={0x01};
    sendCommand(payload_cmd, payload, sizeof(payload));
  }
  else if (payload_cmd == CMD_SET_DIMMING_PARAMETERS)
  {
    Serial.printf("dimmer: received packet %s\n", hexToStr(payload,payload_size));
    Serial.printf("Set dimming params\n");
    uint8_t payload[]={0x01};
    sendCommand(payload_cmd, payload, sizeof(payload));
  }
  else if (payload_cmd == CMD_SET_DIMMING_TYPE_2)
  {
    Serial.printf("dimmer: received packet %s\n", hexToStr(payload,payload_size));
    Serial.printf("Set dimming calibration 1\n");
    uint8_t payload[]={0x01};
    sendCommand(payload_cmd, payload, sizeof(payload));
  }
  else if (payload_cmd == CMD_SET_DIMMING_TYPE_3)
  {
    Serial.printf("dimmer: received packet %s\n", hexToStr(payload,payload_size));
    Serial.printf("Set dimming calibration 2\n");
    uint8_t payload[]={0x01};
    sendCommand(payload_cmd, payload, sizeof(payload));
  }
  else
  {
    Serial.printf("dimmer: received packet %s\n", hexToStr(payload,payload_size));
    Serial.printf("Unknown command: 0x%02X\n", payload_cmd);
  }
}

uint16_t crc(uint8_t *buffer, uint8_t len) {
  uint16_t c = 0;
  for (int i = 1; i < len; i++) {
    c += buffer[i];
  }
  return c;
}

void receivePacket() {
  while (Serial1.available() > 0)
  {
    uint8_t b = Serial1.read();
    //Serial.printf("rx_idx: %d  byte: %02X\n", rx_idx, b);
    rx_buffer[rx_idx] = b;

    if (rx_idx == 0 && b != _packet_start_marker) { // start marker
      Serial.printf("dimmer: received wrong start marker: 0x%02X\n", b);
      rx_idx = 0;
      continue;
    }

    if (rx_idx == buffer_size - 1) {
      Serial.println(F("dimmer: rx buffer overflow"));
      rx_idx = 0;
      continue;
    }

    if (rx_idx == 1 && b != _packet_counter - 1) { // packet counter is same as previous tx packet
      //Serial.printf("dimmer: packet counter seems to be wrong: 0x%02X\n", b);
      //rx_idx = 0;
      //continue;
    }

    if (rx_idx == 2) { // command
      rx_payload_cmd = b;
    }

    if (rx_idx == 3) { // payload size
      rx_payload_size = b;
      if (rx_payload_size > rx_max_payload_size)
        Serial.printf("dimmer: overflow with payload size %d\n", rx_payload_size);
    }

    if (rx_idx == (3 + rx_payload_size + 2)) { // checksum
      uint16_t c = (rx_buffer[rx_idx - 1] << 8) + b;
      if (c != crc(rx_buffer, rx_idx - 1)) {
        Serial.printf("dimmer: received wrong checksum at rx_idx: %d\n", rx_idx);
        rx_idx = 0;
        continue;
      }
      // Copy the payload
      if (rx_payload_size > 0)
      {
        memcpy(rx_payload, &rx_buffer[rx_idx - 1 - rx_payload_size], rx_payload_size);
      }
    }

    if (rx_idx == (3 + rx_payload_size + 3) && b != _packet_end_marker) { // end marker
      Serial.printf("dimmer: received wrong end marker: 0x%02X\n", b);
      rx_idx = 0;
      continue;
    }

    if (rx_idx == (3 + rx_payload_size + 3)) { // end marker
      //Serial.println(F("dimmer: received packet"));
      //hexToStr(rx_buffer, rx_idx + 1);
      // Process the packet which has just been received
      processReceivedPacket(rx_payload_cmd, rx_payload, rx_payload_size);
      rx_idx = 0;
      continue;
    }

    rx_idx++;
  }
}

void loop()
{
  receivePacket();
}

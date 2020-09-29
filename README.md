# Shelly-Dimmer-2-Reverse-Engineering

Some tools for the reverse engineering of the Shelly Dimmer 2.

<a href="https://github.com/Mollayo/Shelly-Dimmer-2-Reverse-Engineering/blob/master/serialSniffer.ino">serialSniffer.ino</a>: This code runs on ESP32. Its purpose is to capture the Tx data from both the ESP8266 and the STM32 on the Shelly Dimmer 2. These data can then be used to analyse the communication protocole between the two MCU.

<a href="https://github.com/Mollayo/Shelly-Dimmer-2-Reverse-Engineering/blob/master/fakeSTM32.ino">fakeSTM32.ino</a>: This code also works on ESP32. This is to emulate the STM32 inside the Shelly Dimmer 2. The ESP32 running this code is connected to a generic ESP8266 on which the stock firmware of the Shelly Dimmer 2 has been installed. It is then possible to access to the Shelly web interface, modify the parameters and see what kind of data is sent to the STM32. If the esp8266 with the Shelly stock firmware is run alone (i.e. without being connected to the STM32), the device will not boot properly and the Shelly web interface will look like <a href="https://github.com/Mollayo/Shelly-Dimmer-2-Reverse-Engineering/blob/master/not_booting_properly.jpg">this</a>.

The stock firmware of the Shelly Dimmer 2 can be found here https://github.com/Mollayo/Shelly-Dimmer-2-Reverse-Engineering/tree/master/shelly%20stock%20firmware.


Command to backup a firmware: 
    esptool.py --port /dev/ttyUSB0 read_flash 0x00000 0x200000 shelly_dimmer_2_backup.bin

Command to restore a firmware: 
    esptool.py --port /dev/ttyUSB0 write_flash 0x00000 shelly_dimmer_2_backup.bin --flash_size 2MB


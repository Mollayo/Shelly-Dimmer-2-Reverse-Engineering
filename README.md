# Shelly-Dimmer-2-Reverse-Engineering

Some tools for the reverse engineering of the Shelly Dimmer 2.

serialSniffer.ino: This code runs on ESP32. It is able to capture the Tx data from both the esp8266 and the STM 32 on the Shelly Dimmer 2. These data can be then used to analyse the communication protocole between the two MCU.

fakeSTM32.ino: This code also works on ESP32. This is to emulate the STM32 inside the Shelly Dimmer 2. The ESP32 running this code is connected to a generic esp8266 on which the stock firmware of the Shelly dimmer 2 has been installed. Then, it is possible to access to the shelly web interface, modify the parameters and see what kind of data is sent to the STM MCU.

The stock firmware of the Shelly Dimmer 2 can be found here: https://www.shelly-support.eu/filebase/index.php?entry-download/138-shelly-dimmer-2-recovery-fw-1-8-0/&fileID=190.

# Ocelot Telemetry
### Data Collection for the Mojave Sphinx

## Overview
The Arduino code collects data from sensors, writes the values to a SD card and sends them over the network. The python listener receives the network messages, writes them to a file and prints them to the terminal. Python plotting template code is also included.

## Installation
To install, download the GitHub repository as a zip file, or clone it with git (cloning is prefered since it will be more convenient).

```bash
git clone git@github.com:LPRD/MojaveSphinx.git
```

To program the Arduino, downolad the [Arduino IDE](https://www.arduino.cc/en/software/). Open the repository as a sketch. All the needed libraries should be installed by default. Select the board to compile to. Currently we are using the Mega. Alternatively, plug the Arduino board into your computer with a USB cable, and the IDE should auto detect the right board.

To receive data over the network, configure your computer's IP address when connected to ethernet to be `192.169.1.2`. The IP mask can be `255.255.255.0`. On windows go to Settings > Network & Internet > Ethernet and Edit the IP address. Then run the listener with python. 
```bash
python listener.py
```
The ethernet may need to be plugged in before your computer will bind to the IP address.

## Code Behavior Notes
- The code will search for an available filename on the SD card. The default file name is DATA.txt. Subsequent files will be named DATA1.txt, DATA2.txt, ...
- The SD card flushes every 5 seconds. If the board loses power before it flushes, the data will be lost.
- The code will not run if a SD card is not inserted. If the SD card is inserted after the board is powered on, it will still not run. Restart the board.
- When the code is running, a beeper sounds periodically (3 seconds). It is helpful for determining if the SD card was recognized.
- Data is saved as text data. This is convenient but takes more space. If this is an issue, consider storing the data in binary.
- The load cell is not tared. Instead a offset has been found ahead of time, that should set set its zero point correctly. This allows the load cell to report forces that are already present when the Arduino is powered on, instead of only changes in force.
- The HX711 board has been modified to be in fast mode, which introduces more noise. A median filter in the code removes some of this noise, but it can also destroy or delay legitimate data.

# Get data from Votronic SR Duo Digital solar chargers

I am using a Votronic SR 300 Duo Digital with serial no. 13v20.0121 which is not capable of connecting the Votronic bluetooth devices (supported starting with serial no. 14Vxx.xxxxx):

![Votronic SR 300 Duo Digital Picture](extras/Votronic_SR_300_Duo_Digital_installed.jpg?raw=true "Votronic SR 300 Duo Digital Picture")

In my RV it is connected to a Votronic Solar Computer S 1250 (again not capable of the bluetooth stuff which started with serial no. 15V40.xxxxx):

![Votronic Solar Computer S 1250 Picture](extras/Votronic_SolarComputer_S_1250.jpg?raw=true "Votronic Solar Computer S 1250 Picture")

So I tried connecting a logic analyzer and found some flanks with the smaller ones beeing 980µs apart which seemed to be a bit length. I added a UART analyzer with 1020baud (1 / 0.000980 = 1020) and now I was able to see some bytes without any timing errors:

![Votronic logic analyzer capture Picture](extras/Votronic_SR_300_Duo_Digital_protocol_capture.jpg.png?raw=true "Votronic logic analyzer capture Picture")

To verify the voltage I attached a digital storage oscilloscope and found a pretty solid 5V level output (1 y-unit = 1V): 

![Votronic DSO capture Picture](extras/Votronic_SR_300_Duo_Digital_protocol_oscilloscope.jpg?raw=true "Votronic DSO capture Picture")

# Sample

Simple sketch for esp32 using hardware serial: [arduinoHardwareSerial.ino](examples/arduinoHardwareSerial/arduinoHardwareSerial.ino).

No device at hand? Try: [arduinoNoDevice.ino](examples/arduinoNoDevice/arduinoNoDevice.ino).


# Pinout of the RJ12 6p6c connector

The most relevant pins would be :
1. Serial Data
2. Solar/Main Battery +
3. 
4. GND
5. 
6. 


# Schematics

![Connector pinout schematic Picture](extras/VotronicSRDuoDig_connector_schematic.png?raw=true "Connector pinout schematic Picture")

[Eagle SCH](extras/VotronicSRDuoDig_connector.sch)

![Connector pinout board Picture](extras/VotronicSRDuoDig_connector_board.png?raw=true "Connector pinout board Picture")

[Eagle BRD](extras/VotronicSRDuoDig_connector.brd)
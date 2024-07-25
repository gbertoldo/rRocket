rRocket is an Arduino-powered rocketry recovery system.

Features
========
- Drogue and parachute deployment
- Data stored in EEPROM
- Error report

Arduino Nano benchmarks
=======================
Time to erase completely the EEPROM: 3,4 seconds
Time to read all data from EEPROM: 30 milliseconds
Time to update the altitude vector (13 elements) and check the fly conditions: 0.02 ms
Time to update the altitude vector (25 elements) and check the fly conditions: 0.04 ms
Time to get the altitude from the barometer: 2,15 ms
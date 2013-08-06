Arduino CAN & sensor logger
====================================================

Log CAN & other sensor data to UART & SD card.

Required hardware
-----------------
- Arduino (Uno/Mega?)
- Sparkfun CAN-BUS shield (https://www.sparkfun.com/products/10039)

Required software
-----------------
- Arduino IDE 1.0.5 (tested on Windows 7)


Getting started
===================================================

Arduino IDE build
-----------------

- Get Arduino IDE (1.0.5 on Windows 7 is tested)
- Copy contents of libs dir to Arduino libs dir
 - Note that SD is part of the 1.0.5 IDE, however you need to
   use the later version included in this project for the Canbus
   example to compile.
- Open sketch in Arduino IDE, verify, download, there you go.


Eclipse CDT IDE build (yet to exist)
------------------------------------

- Click "Import existing project" in Eclipse
- Navigate to this directory, click OK. Should see this project. Import.
- Click build
- Run upload.bat to upload to Arduino
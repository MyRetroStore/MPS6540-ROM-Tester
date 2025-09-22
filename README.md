# MPS6540 ROM Tester

Arduino shield to read and verify MPS6540 ROM's used in the Commodore PET.

## Introduction
Modern EPROM programmers like the TL866 are unable to read obsolete MPS6540 ROM chips used in Commodore PET's.
The MPS6540 ROMs were manufactureded by Commodore and used 5 (Yes...5!) Chip Select lines. 

I built a shield for the Arduino Mega 2560 which reads the ROM and compares the MD5 hash value to know good ROMs found on Zimmers.net with a option of displaying the contents of the ROM via the Arduino IDE serial monitor. 

![Serial Output](https://raw.githubusercontent.com/MyRetroStore/MPS6540-ROM-Tester/main/hardware/mps6540-top.png)

## Usage
This Arduino sketch was built and tested using Arduino IDE 1.8.19 but should work with later versions.

Before uploading the .ino to the Mega2560, review the config.h file and change the paramaters as required:

To display ROM contents set ReadOnly to true
Once the code has been uploaded insert the chip (Remove power before inserting the ROM and take note of Pin 1 orientation) and open the serial monitor set to 115200 baud.

![Serial Output](https://raw.githubusercontent.com/MyRetroStore/MPS6540-ROM-Tester/main/hardware/mps6540.png)

Program will run and compare the md5 hash of the ROM under test, or display the contents of the ROM.

![Serial Output](https://raw.githubusercontent.com/MyRetroStore/MPS6540-ROM-Tester/main/hardware/mps6540-serial.png)

The Arduino code comes pre-configured with a set of md5 hashes from most of the Commodore ROMs, but additional hashes can be added using the supplied python [script](https://github.com/MyRetroStore/MPS6540-ROM-Tester/tree/main/software)

## Shield

The gerber files for the shield can be found under hardware/gerbers

![Retro ROM Tester](https://github.com/MyRetroStore/MPS6540-ROM-Tester/blob/main/hardware/mps6540-hat.png?raw=true)

## BOM
| Reference | Value |
| ------ | ------ |
| PASS | Green LED 0805 |
| FAIL | Red LED 0805 |
| R1, R2 | 1k Resistor 0805 |
| U1 | 40 Pin ZIF Socket |
| SW1 | Push Button Switch 6x6x6 |
| | 2.54mm SIL Male Headers

## Purchasing

If you don't want build the shield yourself, assembled shields can be purchased from:
- [MyRetroStore](https://myretrostore.co.uk/)
- [eBay](https://www.ebay.co.uk/str/myretrostoreuk)

## License
CC BY-NC-SA 4.0


[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/H2H8RDX9W)

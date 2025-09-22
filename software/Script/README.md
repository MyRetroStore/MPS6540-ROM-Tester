# Creating MD5 hashes

The python script was developed in Linux, but should run on any OS with python 3.x

## Introduction

The python script **create_md5.py** is used to generate MD5 hashes from ROM files. This is useful if the ROM you are testing does not have a MD5 hash pre-configured in the code. 

As the Arduino Mega 2560 has a limited amount of RAM, when reading the ROM the contents are read in 4k chunks. 
 
 The hashes are stored in the file [crc.h](https://github.com/MyRetroStore/MPS6540-ROM-Tester/blob/main/software/Arduino/crc.h) and take the format of the following array.


```
 2k ROM:
 ```
  {"basic_4_d000_901465_21", "ab780e94772dca756a0678a17b5bc3a2"},
 ```
 
## Usage

First you need to copy ```crc.h``` from the Arduino folder to the same folder as ```create_md5.py``` as this script updates the file with the new hash value.
Then run:
```
./create_md5.py -i filename
```
This calculates the md5 hash for the ROM binary (filename) and updates the crc.h file. 

 Example:
 ```
 ./create_md5.py -i kernal.901227-01.bin
 ```
Once completed copy ```crc.h``` back to the Arduino folder and compile and upload the sketch to the Mega2560.
 
 ## To Do
 - While basic error checking is done in the python script, there is a lot more error checking that could be done so not all errors will be catered for (Eg if you had to remove the marker text from crc.h then the script will probably throw a weird error)
 - I'm not a programmer so while this script seems to work, it's not the most efficient and there are probably a lot better ways to do things. 
 


## License
CC BY-NC-SA 4.0

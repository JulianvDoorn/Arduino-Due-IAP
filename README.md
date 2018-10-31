# Arduino-Due-IAP
**This repository is not maintained**

Current status:

- CMake Files: Generates build/ folder with Kvasir's toolchain
- Makefile: Used to generate "lib.bin", which should be executable code for CM3 chips with thumb instructions
- lib.h lib.c: Can be compiled using make lib.bin, lib.bin is a position independent executable, in theory it can be sent to an Arduino Due. The Due loads it into memory (2 bits aligned). The code should then be able to be executed using a function pointer. (The pointer's binary value must end in a 1, took a week to discover)
- main.cpp: read the code, dammit
- flashtools/: Some files that allow for reflashing pages of the Due's flash, see files for further elaboration
- Flash.py: A big WIP, in theory it should send files in binary over a COM/tty port. Supports up to 256 bytes, if file is less than 256 it pads the data with zeroes.

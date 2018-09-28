#!/usr/bin/python3

from serial import *

port = "COM3"

serial = Serial(
    port=port,
    baudrate=19200,
    bytesize=EIGHTBITS,
    parity=PARITY_NONE,
    stopbits=STOPBITS_ONE,
    timeout=1,
    xonxoff=True,
    rtscts=True,
    dsrdtr=False
)

serial.write(bytearray("Hello world!", "UTF8"))

while True:
    print(serial.read(), end='')
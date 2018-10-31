#!/usr/bin/python3

from serial import Serial, EIGHTBITS, PARITY_NONE, STOPBITS_ONE

from cement.core.foundation import CementApp
from cement.core.controller import CementBaseController, expose

from sys import stdin, stdout

VERSION = "0.0.1"

BANNER = """
Run-time code flashtool v%s
""" % VERSION

class Controller(CementBaseController):
    def openSerial(self, port):
        self.serial = Serial(
            port=port,
            baudrate=19200,
            bytesize=EIGHTBITS,
            parity=PARITY_NONE,
            stopbits=STOPBITS_ONE,
            timeout=1,
            xonxoff=True,
            rtscts=True,
            dsrdtr=False,
            exclusive=False
        )

    class Meta:
        label = "base"
        arguments = [
            (["-v", "--version"], dict(action="version", version=BANNER)),
            (["-i", "--input"], dict(help="Sets the input *.bin file")),
            (["-p", "--port"], dict(help="Sets the input *.bin file")),
            (['vargs'], dict(action='store', nargs='*')),
        ]

    @expose(hide=True)
    def default(self):
        if not self.app.pargs.vargs and not self.app.pargs.input:
            exit("No input file specified")

        input = self.app.pargs.input or self.app.pargs.vargs[0]

        if input[-4:] != ".bin":
            print("Given file is not a .bin file")
            print("Do you want to continue? Y/N: ", end='')
            stdout.flush()
            c = stdin.read(1)

            if c.lower() != 'y':
                exit("Abort")

        if not self.app.pargs.port:
            exit("No port specified")
        
        self.openSerial(self.app.pargs.port)

        bytesSent = 0

        with open(input, "rb") as fin:
            bytes = bytearray(fin.read(256))
            bytesSent += len(bytes)

            self.serial.write(bytes)

        self.serial.write(bytearray([0] * (256 - bytesSent % 256))) # fill flash buffer till end
        
class App(CementApp):
    class Meta:
        label = "flash"
        base_controller = Controller


with App() as app:
    app.run()
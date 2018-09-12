#include <hwlib.hpp>

extern "C" {
    using libraryFunction = int(*)();

    const char f[0xFF] __attribute__((aligned(4))) = "\x10\xB5\x05\x4C\x7C\x44\x01\x3C\x14\xF8\x01\x0F\x00\xB9\x10\xBD"
    "\x00\xF0\x04\xF8\xF8\xE7\x00\xBF\x24\x00\x00\x00\x02\x4B\x5A\x69"
    "\x92\x07\xFC\xD5\xD8\x61\x70\x47\x00\x08\x0E\x40\x48\x65\x6C\x6C"
    "\x6F\x20\x77\x6F\x72\x6C\x64\x21\x0A\x4A\x75\x6C\x69\x61\x6E\x20"
    "\x69\x73\x20\x61\x77\x65\x73\x6F\x6D\x65\x0A\x00";

    int main() {
        WDT->WDT_MR = WDT_MR_WDDIS;

        hwlib::wait_ms(1000);

        hwlib::target::uart_init();

        libraryFunction fun = (libraryFunction) f;

        int c = fun();

        hwlib::cout << c << hwlib::endl;
    }
}
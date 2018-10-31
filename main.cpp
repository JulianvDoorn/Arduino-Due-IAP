#include <hwlib.hpp>
#include <sam3x8e.h>
#include <core_cm3.h>

#include "flashtools/FlashController.hpp"

using functionPtr = void(*)();

volatile static const constexpr char __attribute__((section(".text"))) __attribute__((aligned(256))) flashCode[Generic_Flash_Traits::IFLASH_PAGE_SIZE] = { };

template <class T>
void __attribute__ ((noinline)) print(T v) {
	hwlib::cout << v;
	hwlib::cout << hwlib::endl;
}

template <class T>
void __attribute__ ((noinline)) print_no_endl(T v) {
	hwlib::cout << v;
}

template <class... T>
void __attribute__ ((noinline)) print(T... v) {
	print_no_endl(v...);
	hwlib::cout << hwlib::endl;
}

template <class... T>
void __attribute__ ((noinline)) print_no_endl(T... v) {
	print_no_endl(v...);
}

int main() {
	WDT->WDT_MR = WDT_MR_WDDIS;

	hwlib::wait_ms(1000);

	print("Starting...");

	Flash_Controller flashController;

	static const constexpr char ramData[Generic_Flash_Traits::IFLASH_PAGE_SIZE] = 
		"\x10\xB5\x05\x4C\x7C\x44\x01\x3C\x14\xF8\x01\x0F\x00\xB9\x10\xBD"
		"\x00\xF0\x04\xF8\xF8\xE7\x60\xBF\x24\x00\x00\x00\x02\x4B\x5A\x69"
		"\x92\x07\xFC\xD5\xD8\x61\x70\x47\x00\x08\x0E\x40\x48\x65\x6C\x6C"
		"\x6F\x20\x77\x6F\x72\x6C\x64\x21\x0A\x4A\x75\x6C\x69\x61\x6E\x20"
		"\x69\x73\x20\x61\x77\x65\x73\x6F\x6D\x65\x0A\x00";

	if (flashCode[0] != 0x00) {
		///< Creates a function pointer to the flash memory. Instructions are in Thumb format, so the last bit of the instruciton must be 1
		///< That explains why there's `| 0b01` manipulating the address of the function pointer.
		functionPtr fun = (functionPtr) (((uint32_t) flashCode) | 0b01);
		fun();
	} else {
		// Should be able to be turned constexpr, somehow
		uint16_t regionNumber = Region_Ptr::make(flashCode).getRegionNumber();

		flashController.unlockRegion(regionNumber);
		flashController.writeFlashPage((uint32_t*) flashCode, (uint32_t*) ramData, Generic_Flash_Traits::IFLASH_PAGE_SIZE);
		flashController.lockRegion(regionNumber);

		for (int i = 0; i < Generic_Flash_Traits::IFLASH_PAGE_SIZE; i++) {
			hwlib::cout << hwlib::hex << hwlib::setw(2) << hwlib::setfill('0');
			print_no_endl((int) flashCode[i]);
		}
		hwlib::cout << hwlib::endl;
	}
}

//int main() {
//    WDT->WDT_MR = WDT_MR_WDDIS;
//
//    hwlib::wait_ms(1000);
//
//    int offset = reinterpret_cast<int>(f) % 0b11;
//
//    hwlib::cout << offset << hwlib::endl;
//
//    while (1) {
//        int i = 0;
//
//        while (i < 0xFF - offset) {
//            hwlib::cin >> f[i + offset];
//
//            i++;
//        }
//
//        hwlib::cout << "flash buffer full!" << hwlib::endl;
//        // hwlib::cout << "writing to rom" << hwlib::endl;
//
//        hwlib::cout << "executing code!" << hwlib::endl;
//
//        // for (int i = 0; i < 0xFF; i++) {
//        //     hwlib::cout << i << " " << flashBuffer[i] << hwlib::endl;
//        // }
//    // }
//
//        libraryFunction fun = (libraryFunction) (f + offset);
//
//        for (int i = 0; i < 100; i += 2) {
//            hwlib::cout << hwlib::setfill('0') << hwlib::setw(2) << hwlib::hex << (int) f[i];
//            hwlib::cout << hwlib::setfill('0') << hwlib::setw(2) << hwlib::hex << (int) f[i + 1] << hwlib::endl;
//        }
//
//        int c = fun();
//    }
//
//    // fun();
//
//    // int c = fun();
//
//    // hwlib::cout << c << hwlib::endl;
//}
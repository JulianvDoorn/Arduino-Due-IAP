#include "IAP.hpp"

IAP_Command::IAP_Command(FlashCommands command, uint16_t param) : command(static_cast<uint8_t>(command)), param(param), key(0x5A) {	}

uint32_t PerformIAP(uint32_t efc, FlashCommands command, uint16_t param) {
	using IAP_Function_t = uint32_t(*)(uint32_t efc, IAP_Command cmd);
	static const IAP_Function_t IAP_Function = *reinterpret_cast<IAP_Function_t*>(IROM_ADDR + 0x08);

	return IAP_Function(efc, { command, param });
}
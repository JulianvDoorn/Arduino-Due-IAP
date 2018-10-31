#pragma once

#include <sam3x8e.h>

/**
 * @brief Enum class for the different types of flash commands supported by the EFC
 * 
 * @details
 *  Command description					| ID   | Short name | Param
 *  ------------------------------------+------+------------+-------------
 *  Get Flash Descriptor				| 0x00 | GETD		| None
 *  Write page							| 0x01 | WP			| Page number
 *  Write page and lock					| 0x02 | WPL		| Page number
 *  Erase page and write page			| 0x03 | EWP		| Page number
 *  Erase page and write page then lock | 0x04 | EWPL		| Page number
 *  Erase all							| 0x05 | EA			| None
 *  Erase plane							| 0x06 | EPL		| Page number
 *  Set Lock Bit						| 0x08 | SLB		| Page number
 *  Clear Lock Bit						| 0x09 | CLB		| Page number
 *  Get Lock Bit						| 0x0A | GLB		| None
 *  Set GPNVM Bit						| 0x0B | SGPB		| GPNVM number
 *  Clear GPNVM Bit						| 0x0C | CGPB		| GPNVM number
 *  Get GPNVM Bit						| 0x0D | GGPB		| None
 *  Start Read Unique Identifier		| 0x0E | STUI		| None
 *  Stop Read Unique Identifier			| 0x0F | SPUI		| None
 */
enum class FlashCommands {
	GETD = 0x00,  //!< Get Flash Descriptor
	WP   = 0x01,  //!< Write page
	WPL  = 0x02,  //!< Write page and lock
	EWP  = 0x03,  //!< Erase page and write page
	EWPL = 0x04,  //!< Erase page and write page then lock
	EA   = 0x05,  //!< Erase all
	SLB  = 0x08,  //!< Set Lock Bit
	CLB  = 0x09,  //!< Clear Lock Bit
	GLB  = 0x0A,  //!< Get Lock Bit
	SGPB = 0x0B,  //!< Set GPNVM Bit
	CGPB = 0x0C,  //!< Clear GPNVM Bit
	GGPB = 0x0D,  //!< Get GPNVM Bit
	STUI = 0x0E,  //!< Start unique ID
	SPUI = 0x0F  //!< Stop unique ID
};

/**
 * @brief IAP_Command struct, identical memory layout as EEFC_FCR
 * 
 * @details
 * The IAP_Command struct has the same memoryy layout as the EEFC_FCR
 * register. (Enhanced Embedded Flash Controller Flash Command Register)
 * That way, when calling the IAP_Routine, it can be casted into an 
 * uint32_t and call the routine located in ROM.
 */
class IAP_Command {
	unsigned command : 8;
	unsigned param : 16;

	///< The Flash Writing Protection key prevents the IAP from being called accidentally.
	///< If the key is not set to 0x5A, the IAP routine blocks the call. The key is located
	///< in the final 8 bits of the flash command register. 
	unsigned key : 8;

public:
	IAP_Command(FlashCommands command, uint16_t param);
};

static_assert(sizeof(IAP_Command) == sizeof(uint32_t), "IAP_Command must be exactly 32 bits long");

uint32_t PerformIAP(uint32_t efc, FlashCommands command, uint16_t param);
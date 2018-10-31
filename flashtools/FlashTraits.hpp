#pragma once

#include <sam3x8e.h>
#include <core_cm3.h>

/**
 * @brief Template definition for internal _EFC_Traits class
 * 
 * @tparam BANK_NUM Index of the Embedded Flash Controller (EFC)
 */
template <int32_t BANK_NUM> class Flash_Traits;

/**
 * @brief Template specialization for bank 0
 */
template <>
class Flash_Traits<0> {
public:
    // static constexpr const Efc* EFC = static_cast<Efc*>(EFC0);
    static constexpr const uint32_t EFC_ID = 0;
    static constexpr const uint32_t IFLASH_PAGE_SIZE = IFLASH0_PAGE_SIZE;
    static constexpr const uint32_t IFLASH_ADDR = IFLASH0_ADDR;
    static constexpr const uint32_t IFLASH_LAST_PAGE_ADDRESS = IFLASH0_ADDR + IFLASH0_SIZE - IFLASH0_PAGE_SIZE;
    static constexpr const uint32_t IFLASH_LOCK_REGION_SIZE = IFLASH0_LOCK_REGION_SIZE;
};

/**
 * @brief Template specialization for bank 1
 */
template <>
class Flash_Traits<1> {
public:
    // static constexpr const Efc* EFC = EFC1;
    static constexpr const uint32_t EFC_ID = 1;
    static constexpr const uint32_t IFLASH_PAGE_SIZE = IFLASH1_PAGE_SIZE;
    static constexpr const uint32_t IFLASH_ADDR = IFLASH1_ADDR;
    static constexpr const uint32_t IFLASH_LAST_PAGE_ADDRESS = IFLASH1_ADDR + IFLASH1_SIZE - IFLASH1_PAGE_SIZE;
    static constexpr const uint32_t IFLASH_LOCK_REGION_SIZE = IFLASH1_LOCK_REGION_SIZE;
};

/**
 * @brief Template specialization for a generic flash bank
 * 
 * @details
 * Traits that are not shared by every bank is omitted in
 * order to prevent mistakes.
 */
template <>
class Flash_Traits<-1> {
public:
    static constexpr const uint32_t IFLASH_PAGE_SIZE = IFLASH0_PAGE_SIZE;
    static constexpr const uint32_t IFLASH_LOCK_REGION_SIZE = IFLASH0_LOCK_REGION_SIZE;
    static constexpr const uint32_t IFLASH_ADDR = IFLASH0_ADDR;
};

using Generic_Flash_Traits = Flash_Traits<-1>;
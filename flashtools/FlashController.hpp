/**
 * @brief Embedded Flash Controller driver for Atmel SAM3X8E
 * 
 * @file FlashController.hpp
 * @author Julian van Doorn
 * @date 2018-09-17
 */

#pragma once

#include <cstring>

#include <sam3x8e.h>
#include <core_cm3.h>

#include "FlashTraits.hpp"
#include "IAP.hpp"

enum class Flash_Status {
    SUCCESS = 0x00, ///< Successful flash
    BUFFER_LENGTH_NOT_SUBSTITUTABLE ///< Buffer len not dividable by 4
};

/**
 * @brief Page_Ptr type
 * 
 * @details
 * The purpose of this class is making it clear that the input
 * pointer has to point to a page. It can also be used to align
 * pointers correctly.
 */
class Page_Ptr {
    void* ptr;

public:
    /**
     * @brief Makes and aligns a Page_Ptr based on ptr
     * 
     * @param ptr Pointer to make a Page_Ptr with
     * @return constexpr Page_Ptr Aligned ptr
     */
    template <class T>
    static constexpr Page_Ptr make(T& val) {
        Page_Ptr pagePtr((void*) &val);
        pagePtr.align();
        return pagePtr;
    }

    template <class T>
    constexpr Page_Ptr(T* ptr) : ptr(reinterpret_cast<void*>(ptr)) { }

    constexpr void align() {
        ptr = reinterpret_cast<uint32_t*>(reinterpret_cast<uint32_t>(ptr) & ~(Generic_Flash_Traits::IFLASH_PAGE_SIZE - 1));
    }

    uint32_t getPageNumber() {
        return (reinterpret_cast<uint32_t>(ptr) - Generic_Flash_Traits::IFLASH_ADDR) / Generic_Flash_Traits::IFLASH_PAGE_SIZE;
    }

    operator uint32_t*() {
        return reinterpret_cast<uint32_t*>(ptr);
    }

    explicit operator uint32_t() {
        return reinterpret_cast<uint32_t>(ptr);
    }
};

/**
 * @brief Region_Ptr type
 * 
 * @details
 * The purpose of this class is making it clear that the input
 * pointer has to point to a region. It can also be used to align
 * pointers correctly.
 */
class Region_Ptr {
    void* ptr;

public:
    /**
     * @brief Makes and aligns a Page_Ptr based on ptr
     * 
     * @param ptr Pointer to make a Page_Ptr with
     * @return constexpr Page_Ptr Aligned ptr
     */
    template <class T>
    static constexpr Region_Ptr make(T& val) {
        Region_Ptr regionPtr((void*) &val);
        regionPtr.align();
        return regionPtr;
    }

    template <class T>
    constexpr Region_Ptr(T* ptr) : ptr(reinterpret_cast<void*>(ptr)) { }

    constexpr void align() {
        ptr = reinterpret_cast<uint32_t*>(reinterpret_cast<uint32_t>(ptr) & ~(Generic_Flash_Traits::IFLASH_LOCK_REGION_SIZE - 1));
    }

    uint32_t getRegionNumber() {
        return (reinterpret_cast<uint32_t>(ptr) - Generic_Flash_Traits::IFLASH_ADDR) / Generic_Flash_Traits::IFLASH_LOCK_REGION_SIZE;
    }

    operator uint32_t*() {
        return reinterpret_cast<uint32_t*>(ptr);
    }

    explicit operator uint32_t() {
        return reinterpret_cast<uint32_t>(ptr);
    }
};

class Flash_Controller {
public:
    /**
     * @brief Takes the specified region and unlocks it
     * 
     * @details
     * 
     * 
     * @param regionNumber Region to unlock
     * @return uint32_t IAP Status
     */
    uint32_t unlockRegion(uint16_t regionNumber) {
        return PerformIAP(1, FlashCommands::CLB, regionNumber);
    }

    /**
     * @brief Takes the specified region and locks it
     * 
     * @details
     * 
     * @param regionNumber Region to lock
     * @return uint32_t IAP Status
     */
    uint32_t lockRegion(uint16_t regionNumber) {
        return PerformIAP(1, FlashCommands::SLB, regionNumber);
    }

    /**
     * @brief Flashes the page of destFlash with the content of the srcBuffer
     * 
     * @details
     * The page is determined by aligning destFlash with 0x100 (size of page).
     * That way a page pointer is created and the routine knows where to write.
     * One page can be flashed at a time, so if destFlash has an offset from
     * the begin of the page, then fall-off bytes cannot be flashed. It is
     * undefined what happens when a srcBuffer is flashed which partially
     * falls off the page.
     * 
     * @param destFlash Pointer to the destination flash memory
     * @param srcBuffer Source buffer to copy information from
     * @param bufferLen Length of the source buffer in bytes.
     * @return uint32_t Status code
     */
    Flash_Status writeFlashPage(uint32_t* destFlash, uint32_t* srcBuffer, uint32_t bufferLen) {
        if (bufferLen & 0b11 != 0x00) {
            return Flash_Status::BUFFER_LENGTH_NOT_SUBSTITUTABLE;
        }

        ///< Pointer to the first byte of the page to write to
        Page_Ptr pagePtr = Page_Ptr::make(*destFlash);
        ///< Target flash memory to write buffer to
        uint32_t bufferTargetBegin = (uint32_t) destFlash;
        ///< Pointer to the first word after the buffer in flash
        uint32_t bufferTargetEnd = ((uint32_t) destFlash) + bufferLen + sizeof(uint32_t);
        ///< Amount of bytes after the target flash memory
        size_t padding = Generic_Flash_Traits::IFLASH_PAGE_SIZE - (bufferTargetEnd - ((uint32_t) pagePtr));
        ///< Amount of bytes before target flash memory
        size_t offset = bufferTargetBegin - ((uint32_t) pagePtr);

        ///< Copy existing flash bytes before the buffer to write into working buffer
        memcpy((void*) pagePtr, (void*) pagePtr, offset);

        ///< Copy buffer into the working buffer
        memcpy((void*) bufferTargetBegin, (void*) srcBuffer, bufferLen);

        ///< Copy existing flash bytes after the buffer to write into working buffer
        memcpy((void*) bufferTargetEnd, (void*) bufferTargetEnd, padding);

        PerformIAP(0, FlashCommands::EWP, pagePtr.getPageNumber());

        return Flash_Status::SUCCESS;
    }
};
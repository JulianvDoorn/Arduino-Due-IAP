include(Kvasir.cmake)
project(IAP_Runtime_Programming)
cmake_minimum_required(VERSION 3.0.0)

set(sources main.cpp)

include(flashtools/FlashTools.cmake)

include(Target.cmake)
include(Hwlib.cmake)

target_link_libraries(${PROJECT_NAME}.elf hwlib)

add_custom_target(
    lib.o
    COMMAND ${COMPILER} -c lib.c -Os -Ideps --std=c11 -fPIC -mcpu=cortex-m3 -mthumb -fomit-frame-pointer -march=armv7-m -fno-exceptions
)

add_custom_target(
    lib.bin
    COMMAND ${OBJCOPY} lib-resolved.o lib.bin -O binary
    DEPENDS lib.o
)
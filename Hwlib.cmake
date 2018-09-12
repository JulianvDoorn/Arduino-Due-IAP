set(hwlib lib/hwlib)
include_directories(${hwlib}/library)

add_library(hwlib STATIC ${hwlib}/library/hwlib.cpp)
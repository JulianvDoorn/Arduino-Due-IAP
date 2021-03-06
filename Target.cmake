# Set compilation traits
set(warnings
"-Wall"
"-Wextra"
#"-pedantic-errors"
"-Wfatal-errors"
"-Wcast-align"
"-Wmissing-declarations"
"-Wredundant-decls"
"-Wuninitialized"
"-Wno-unused-parameter"
"-Wno-missing-field-initializers")

set(cxxflags
"-std=c++17"
"-Os"
# "-fkeep-inline-functions"
# "-fno-inline"
${warnings})

string(REGEX REPLACE ";" " " cxxflags "${cxxflags}")
string(REGEX REPLACE ";" " " ldflags "${ldflags}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${cxxflags}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${ldflags}")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

# Set hwlib target
add_definitions (-DBMPTK_TARGET_arduino_due
                 -DBMPTK_TARGET=arduino_due
                 -DBMPTK_BAUDRATE=19200)

# Set kvasir target
set (cxxflags
    "-Os"
    "-ffunction-sections"
    "-fdata-sections"
    "-fno-exceptions"
    "-fno-rtti"
    "-fno-threadsafe-statics"
    "-specs=nano.specs"
    "-specs=nosys.specs"
    # "-fkeep-inline-functions"
    # "-fno-inline"
    )

string (REGEX REPLACE ";" " " cxxflags "${cxxflags}")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${cxxflags}")
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--defsym=STACK_SIZE=92160")

# The target chip that is compiled for:
include (${toolchain}/targets/arm32/cm3/atmel/sam3x/sam3x8e/sam3x8e.cmake)

# The flashscript used to flash the binary (optional):
include (${toolchain}/targets/arm32/cm3/atmel/sam3x/sam3x8e/flashscripts/arduino_due.cmake)
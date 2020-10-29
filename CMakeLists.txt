cmake_minimum_required(VERSION 3.4)

project(blah)

set( CMAKE_CXX_FLAGS "-mno-red-zone -fpie -mx32 -ggdb -ffreestanding -fno-exceptions -fno-rtti --std=c++14 -fomit-frame-pointer")
set( CMAKE_C_FLAGS " -mno-red-zone -fpie -mx32 -ggdb -ffreestanding -fomit-frame-pointer")

set(SRCS
        boot.s
        debug.cpp
        types.hpp)

set_property(SOURCE boot.s PROPERTY LANGUAGE C)


#add_subdirectory(modules)

add_executable(boot ${SRCS} link.ld)
set_property(TARGET boot PROPERTY LANGUAGE CXX)
set_target_properties(boot PROPERTIES
    LINK_FLAGS "-mx32 -T${CMAKE_SOURCE_DIR}/link.ld -nostdlib -fno-builtin -nostartfiles -nodefaultlibs -z max-page-size=0x1000"
    COMPILE_FLAGS "-fomit-frame-pointer -mno-red-zone -fpie -ggdb -mx32 -fno-builtin -fno-rtti -nostdlib -mcmodel=small")

add_custom_target(run
    COMMAND qemu-system-x86_64 -enable-kvm -machine q35 -cpu Haswell-noTSX -smp 1 -L /etc/bios/ -serial stdio -vga qxl -kernel boot 
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS boot
)

add_custom_target(dbg
    COMMAND qemu-system-x86_64 -enable-kvm -machine q35 -cpu Haswell-noTSX -smp 1 -L /etc/bios/ -serial stdio -vga qxl -kernel boot -s -S
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS boot
)

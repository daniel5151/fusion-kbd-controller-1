project(fusion_kbd_controller)
cmake_minimum_required(VERSION 2.8.3)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${fusion_kbd_controller_SOURCE_DIR}/cmake")
set(BIN_DIR ${fusion_kbd_controller_SOURCE_DIR}/bin)

# fusion_kbd_controller source
include_directories(src)
file(GLOB_RECURSE SRC_FILES
  src/*.c
  src/*.h
  src/*.inc
)

# fusion_kbd_controller executable
add_executable(fusion_kbd_controller ${SRC_FILES})

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
      "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# Setup compiler flags for different platforms
set(C_STANDARD 11)
if (${CMAKE_C_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -g")
  set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -O2 -g")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}") # add more later?

  # suppress warnings
  target_compile_definitions(fusion_kbd_controller PRIVATE _CRT_SECURE_NO_WARNINGS)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ") # add more later?

  # add icon manifest to windows builds
  set(SRC_FILES ${SRC_FILES} resources/icons/fusion_kbd_controller.rc)
endif()

# dependencies

find_path(LIBUSB_INCLUDE_DIR
  NAMES libusb.h
  PATH_SUFFIXES "include" "libusb" "libusb-1.0")
find_library(LIBUSB_LIBRARY
  NAMES usb-1.0 usb
  PATH_SUFFIXES "lib" "lib32" "lib64")

include_directories(${LIBUSB_INCLUDE_DIR})
target_link_libraries(fusion_kbd_controller
  ${LIBUSB_LIBRARY})

# install stuff

install(CODE "set(CMAKE_INSTALL_LOCAL_ONLY true)") # no need to install miniz!
install(TARGETS fusion_kbd_controller RUNTIME DESTINATION ${BIN_DIR})

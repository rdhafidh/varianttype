cmake_minimum_required(VERSION 3.10)
project(bignumtest)

set(SRC main.cpp varianttype.cpp lib/src/gtest-all.cc)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
 add_compile_options( -Werror -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -Wno-self-assign-overloaded)
 if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_compile_options(-fstandalone-debug)
 endif()
endif()
if(MSVC)
    add_compile_options(/std:c++17)
endif()
if(ANDROID AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
 add_compile_options( -Werror -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -Wno-self-assign-overloaded)
endif()

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} lib/include lib )

add_executable(variant ${SRC} )

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
target_link_libraries(variant pthread)
endif()

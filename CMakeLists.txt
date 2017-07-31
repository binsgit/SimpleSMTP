cmake_minimum_required(VERSION 2.8)
project(SimpleSMTP)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")

set(SOURCE_FILES
    SimpleSMTP.cpp
    SimpleSMTP.hpp)

add_library(Reimu_SimpleSMTP SHARED ${SOURCE_FILES})
add_library(Reimu_SimpleSMTP_Static STATIC ${SOURCE_FILES})
set_target_properties(Reimu_SimpleSMTP_Static PROPERTIES OUTPUT_NAME Reimu_SimpleSMTP)

install(FILES SimpleSMTP.hpp DESTINATION include/libReimu/SimpleSMTP)
install(TARGETS Reimu_SimpleSMTP Reimu_SimpleSMTP_Static DESTINATION lib)

add_executable(Test Test.cpp)
target_link_libraries(Test Reimu_SimpleSMTP curl)
set_property(SOURCE Test.cpp APPEND_STRING PROPERTY COMPILE_FLAGS " -I. ")
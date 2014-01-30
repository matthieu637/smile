cmake_minimum_required(VERSION 2.8)

#Nom du projet
project(SmileLib CXX C)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(Boost COMPONENTS system serialization filesystem REQUIRED)
find_package(Threads) #semaphore
# find_package(CppUnit)

include_directories(${CMAKE_SOURCE_DIR}/include ${Boost_INCLUDE_DIRS} ${CPPUNIT_INCLUDE_DIR} )


file ( GLOB_RECURSE sources src/bib/*.cpp include/bib/*.hpp src/sml/*.cpp include/sml/*.hpp )
file ( GLOB_RECURSE sources_test src/test/*.cpp src/simu/*.cpp include/test/*.hpp include/simu/*.hpp)

set(CMAKE_CXX_FLAGS "-O2 -Wall -Wextra -std=c++11 -fPIC -Wno-switch") #-g -O2 -DDEBUG
set(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/lib")

set(TYPE_LIB STATIC CACHE STRING "(STATIC or SHARED)")

if(TYPE_LIB MATCHES SHARED)
  add_library(smile SHARED ${sources})
else()
  add_library(smile STATIC ${sources})
endif()

target_link_libraries(smile ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

if(CMAKE_BUILD_TYPE MATCHES Debug)
  set_target_properties(smile PROPERTIES DEBUG_POSTFIX -d)
endif()

add_executable(stest ${sources_test} ${sources})
target_link_libraries(stest ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ) # ${CPPUNIT_LIBRARIES})


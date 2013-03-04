cmake_minimum_required(VERSION 2.8.4)

project(Driver CXX)

set(CMAKE_CXX_FLAGS "-g -DDEBUG -Wall -Wextra -std=c++11 -fPIC -fno-strict-aliasing -DUSE_RANDR_EXT -DGL_GLEXT_PROTOTYPES -D_SVID_SOURCE -D_BSD_SOURCE -DSHM -DHAVE_CONFIG_H")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(SMILE_PATH "${CMAKE_SOURCE_DIR}/../smile/" CACHE PATH "Chemin vers la librarie Bibliothèque")
find_package(Smile REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)

include_directories(include . ../torcs-1.3.4/export/include ${SMILE_INCLUDE_DIRS} )

file ( GLOB_RECURSE HEADER *.hpp)
file ( GLOB_RECURSE SOURCE *.cpp *.c)

add_library(driver SHARED ${HEADER} ${SOURCE})
target_link_libraries(driver ${SMILE_LIBRARY} ${Boost_LIBRARIES} )

#file(RENAME build/libdriver.so build/smile.so)
install(FILES build/libdriver.so DESTINATION /usr/local/lib/torcs/drivers/smile/ RENAME smile.so)

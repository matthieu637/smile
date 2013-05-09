cmake_minimum_required(VERSION 2.8.4)

project(Driver CXX C)

set(CMAKE_CXX_FLAGS "-g -O2 -DDEBUG -Wall -Wextra -std=c++11 -fPIC -Wl,-z,defs")# -Wl,-z,defs") #Used for debug
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

get_filename_component(PARENT_DIR ${CMAKE_SOURCE_DIR} PATH)

set(SMILE_PATH "${PARENT_DIR}/library/" CACHE PATH "Chemin vers la librarie Bibliothèque")
find_package(Smile REQUIRED)
find_package(Boost COMPONENTS system serialization filesystem REQUIRED)
find_package (Threads)
find_package( TorcsLibs REQUIRED )

include_directories(include ${TORCS_INCLUDE} ${SMILE_INCLUDE_DIRS})

file ( GLOB_RECURSE HEADER include/*.hpp)
file ( GLOB_RECURSE SOURCE src/*.cpp)

add_library(driver SHARED ${HEADER} ${SOURCE})
target_link_libraries(driver 	
		${SMILE_LIBRARY} 
		${Boost_LIBRARIES} 
		${CMAKE_THREAD_LIBS_INIT} 
		${TORCS_LIBS_ROBOTTOOLS}
		${TORCS_LIBS_TGFCLIENT}
		)

if(EXISTS /usr/local/bin/torcs)		
  #install 
  install(FILES build/libdriver.so DESTINATION /usr/local/lib/torcs/drivers/smile/ RENAME smile.so)

  #install data
  file(GLOB_RECURSE files "${CMAKE_SOURCE_DIR}/data/*.xml" "${CMAKE_SOURCE_DIR}/data/*.rgb")
  install(FILES ${files} DESTINATION /usr/local/share/games/torcs/drivers/smile/)
else()
  #install 
  install(FILES build/libdriver.so DESTINATION /usr/lib/torcs/drivers/smile/ RENAME smile.so)

  #install data
  file(GLOB_RECURSE files "${CMAKE_SOURCE_DIR}/data/*.xml" "${CMAKE_SOURCE_DIR}/data/*.rgb")
  install(FILES ${files} DESTINATION /usr/share/games/torcs/drivers/smile/)
endif()

# - Try to find CommunsCE
# Once done this will define
#  SMILE_INCLUDE_DIRS
#  SMILE_LIBRARY

set(SMILE_INCLUDE_DIRS ${SMILE_PATH}/include)

#Trouver lib
if(CMAKE_BUILD_TYPE MATCHES Debug)
	find_library( SMILE_LIBRARY
				  NAMES smile-d
				  PATHS 
						"${SMILE_PATH}/lib"
						 "${SMILE_PATH}/lib/Debug"
						 "${SMILE_PATH}/lib/Release"
				)
else()
	find_library( SMILE_LIBRARY
				  NAMES smile 
				  PATHS 
						  "${SMILE_PATH}/lib"
						  "${SMILE_PATH}/lib/Debug"
						  "${SMILE_PATH}/lib/Release"
				)
endif()


include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(Smile  DEFAULT_MSG
                                  SMILE_LIBRARY SMILE_INCLUDE_DIRS)

mark_as_advanced(SMILE_INCLUDE_DIRS SMILE_LIBRARY )


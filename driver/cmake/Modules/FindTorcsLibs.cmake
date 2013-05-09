
# - Try to find CommunsCE
# Once done this will define

#Trouver lib

find_library( TORCS_LIBS_ROBOTTOOLS
			  NAMES robottools 
			  PATHS 
					  "/usr/local/lib/torcs/lib/"
					  "/usr/lib/torcs/lib/"
			)

find_library( TORCS_LIBS_TGFCLIENT
			  NAMES tgfclient 
			  PATHS 
					  "/usr/local/lib/torcs/lib/"
					  "/usr/lib/torcs/lib/"
			)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(TorcsLibs  DEFAULT_MSG
                                  TORCS_LIBS_ROBOTTOOLS TORCS_LIBS_TGFCLIENT)

mark_as_advanced(TORCS_LIBS_ROBOTTOOLS TORCS_LIBS_TGFCLIENT)


# - Find cppunit
# based on a version found at:
# https://id.eesti.ee/trac/browser/cmake/modules/FindCppUnit.cmake
#
# Find the native cppunit includes and library
#
#  CPPUNIT_INCLUDE_DIR - where to find cppunit/Test.h, etc.
#  CPPUNIT_LIBRARIES   - List of libraries when using cppunit.
#  CPPUNIT_FOUND       - True if cppunit found.
#  CPPUNIT_DEFINITIONS - You should add_definitons(${LIBSMI_DEFINITIONS}) before
#                       compiling code that includes cppunit library files.

IF (CPPUNIT_INCLUDE_DIR)
  # Already in cache, be silent
  SET(CPPUNIT_FIND_QUIETLY TRUE)
ENDIF (CPPUNIT_INCLUDE_DIR)

INCLUDE(FindPkgConfig)
IF (PKG_CONFIG_FOUND)
  IF (CPPUNIT_FIND_REQUIRED AND CPPUNIT_FIND_VERSION)
    SET(_PACKAGE_ARGS cppunit>=${CPPUNIT_FIND_VERSION} REQUIRED)
  ELSE ()
    SET(_PACKAGE_ARGS cppunit)
  ENDIF ()
  IF( NOT CPPUNIT_FIND_QUIETLY)
    MESSAGE("XXX use pkg-config --modversion ${_PACKAGE_ARGS} ...")
  ENDIF( NOT CPPUNIT_FIND_QUIETLY)
  PKG_CHECK_MODULES(CPPUNIT ${_PACKAGE_ARGS})
ENDIF (PKG_CONFIG_FOUND)
SET(CPPUNIT_DEFINITIONS ${CPPUNIT_CFLAGS_OTHER})

FIND_PATH(CPPUNIT_INCLUDE_DIR cppunit/Test.h HINTS ${CPPUNIT_INCLUDE_DIRS})

SET(CPPUNIT_NAMES cppunit)
FIND_LIBRARY(CPPUNIT_LIBRARY NAMES ${CPPUNIT_NAMES} HINTS ${CPPUNIT_LIBRARY_DIRS} )

IF (NOT PKG_CONFIG_FOUND)
  SET(CPPUNIT_LIBRARIES ${CPPUNIT_NAMES})
  GET_FILENAME_COMPONENT(CPPUNIT_LIBRARY_DIRS ${CPPUNIT_LIBRARY} PATH)
ENDIF (NOT PKG_CONFIG_FOUND)

# handle the QUIETLY and REQUIRED arguments and set CPPUNIT_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
#FIND_PACKAGE_HANDLE_STANDARD_ARGS(CppUnit DEFAULT_MSG CPPUNIT_LIBRARY
#    CPPUNIT_INCLUDE_DIR CPPUNIT_LIBRARIES CPPUNIT_LIBRARY_DIRS)
#
MARK_AS_ADVANCED( CPPUNIT_LIBRARY CPPUNIT_INCLUDE_DIR CPPUNIT_LIBRARIES
    CPPUNIT_LIBRARY_DIRS CPPUNIT_DEFINITIONS )

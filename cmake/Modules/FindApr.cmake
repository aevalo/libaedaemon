# - Try to find Apache Portable Runtime
# Once done this will define
#  LIBAPR_FOUND - System has Apache Portable Runtime
#  LIBAPR_INCLUDE_DIRS - The Apache Portable Runtime include directories
#  LIBAPR_LIBRARIES - The libraries needed to use Apache Portable Runtime
#  LIBAPR_DEFINITIONS - Compiler switches required for using Apache Portable Runtime

find_package(PkgConfig REQUIRED)
pkg_check_modules(PC_LIBAPR REQUIRED apr-1)
set(LIBAPR_DEFINITIONS ${PC_LIBAPR_CFLAGS_OTHER})

find_path(LIBAPR_INCLUDE_DIR apr.h
          HINTS ${PC_LIBAPR_INCLUDEDIR} ${PC_LIBAPR_INCLUDE_DIRS}
          PATH_SUFFIXES apr-1)

find_library(LIBAPR_LIBRARY NAMES apr-1 libapr-1
             HINTS ${PC_LIBAPR_LIBDIR} ${PC_LIBAPR_LIBRARY_DIRS})

set(LIBAPR_LIBRARIES ${LIBAPR_LIBRARY})
set(LIBAPR_INCLUDE_DIRS ${LIBAPR_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBAPR_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(LibApr DEFAULT_MSG
                                  LIBAPR_LIBRARY LIBAPR_INCLUDE_DIR)

mark_as_advanced(LIBAPR_INCLUDE_DIR LIBAPR_LIBRARY)

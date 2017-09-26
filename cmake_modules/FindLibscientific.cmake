# Try to find libscientific library
# Once done this will define
#  LIBSCIENTIFIC_FOUND - if system found LIBSCIENTIFIC library
#  LIBSCIENTIFIC_INCLUDE_DIRS - The LIBSCIENTIFIC include directories
#  LIBSCIENTIFIC_LIBRARIES - The libraries needed to use LIBSCIENTIFIC
#  LIBSCIENTIFIC_DEFINITIONS - Compiler switches required for using LIBSCIENTIFIC

set(LIBSCIENTIFIC_ROOT_DIR ${LIBSCIENTIFIC_ROOT_DIR} "/usr/" "/usr/local/")
# Uncomment the following line to print which directory CMake is looking in.
#message(status "LIBSCIENTIFIC_ROOT_DIR: " ${LIBSCIENTIFIC_ROOT_DIR})

find_path(LIBSCIENTIFIC_INCLUDE_DIR
    NAMES scientific.h
    PATHS ${LIBSCIENTIFIC_ROOT_DIR}
    PATH_SUFFIXES "include"
    DOC "The LIBSCIENTIFIC include directory"
)

find_library(LIBSCIENTIFIC_LIBRARY
    NAMES scientific
    PATHS ${LIBSCIENTIFIC_ROOT_DIR}
    PATH_SUFFIXES "lib" "lib32" "lib64"
    DOC "The LIBSCIENTIFIC library"
)


include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LOGGING_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(libscientific DEFAULT_MSG LIBSCIENTIFIC_LIBRARY LIBSCIENTIFIC_INCLUDE_DIR)

if (LIBSCIENTIFIC_FOUND)
    set(LIBSCIENTIFIC_LIBRARIES ${LIBSCIENTIFIC_LIBRARY})
    set(LIBSCIENTIFIC_INCLUDE_DIRS ${LIBSCIENTIFIC_INCLUDE_DIR})
    #set(LIBSCIENTIFIC_DEFINITIONS "-DWIN_SCIENTIFIC")
endif()

# Tell cmake GUIs to ignore the "local" variables.
mark_as_advanced(LIBSCIENTIFIC_ROOT_DIR LIBSCIENTIFIC_INCLUDE_DIR LIBSCIENTIFIC_LIBRARY)

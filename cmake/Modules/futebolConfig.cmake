INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_FUTEBOL futebol)

FIND_PATH(
    FUTEBOL_INCLUDE_DIRS
    NAMES futebol/api.h
    HINTS $ENV{FUTEBOL_DIR}/include
        ${PC_FUTEBOL_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    FUTEBOL_LIBRARIES
    NAMES gnuradio-futebol
    HINTS $ENV{FUTEBOL_DIR}/lib
        ${PC_FUTEBOL_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(FUTEBOL DEFAULT_MSG FUTEBOL_LIBRARIES FUTEBOL_INCLUDE_DIRS)
MARK_AS_ADVANCED(FUTEBOL_LIBRARIES FUTEBOL_INCLUDE_DIRS)


# Generated from testSoftExit.pro.

#####################################################################
## testSoftExit Binary:
#####################################################################

qt_internal_add_executable(testSoftExit
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/"
)

## Scopes:
#####################################################################

qt_internal_extend_target(testSoftExit CONDITION WIN32
    SOURCES
        main_win.cpp
    PUBLIC_LIBRARIES
        user32
)

qt_internal_extend_target(testSoftExit CONDITION UNIX
    SOURCES
        main_unix.cpp
)

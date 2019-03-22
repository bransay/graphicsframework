cmake_minimum_required (VERSION 3.0)

project (GraphicsFramework)

if (WIN32)
    set (TARGET_PLATFORM WIN32)
    set (PLATFORM_DEFINES -DWIN32 -D_WINDOWS -D_UNICODE -DUNICODE)
endif()

add_definitions (${PLATFORM_DEFINES})

set (ENTRYPOINTS_SRC 
    src/main_win32.cpp
)
source_group ("entrypoints" FILES ${ENTRYPOINTS_SRC})

set (SRC ${ENTRYPOINTS_SRC})

add_executable (GraphicsFramework ${TARGET_PLATFORM} ${SRC})

# TODO: add more libraries and stuff?

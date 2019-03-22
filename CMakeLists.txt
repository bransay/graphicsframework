# Refer to https://gist.github.com/mbinna/c61dbb39bca0e4fb7d1f73b0d66a4fd1 for best practices
cmake_minimum_required (VERSION 3.0)

project (GraphicsFramework)

if (WIN32)
    set (TARGET_PLATFORM WIN32)
    set (PLATFORM_DEFINES -DWIN32 -D_WINDOWS -D_UNICODE -DUNICODE)
endif()

set (ENTRYPOINTS_SRC 
    src/main_win32.cpp
)
source_group ("entrypoints" FILES ${ENTRYPOINTS_SRC})

set (SRC ${ENTRYPOINTS_SRC})

# TODO: this shouldn't be a target, but it's probably useful for testing
add_executable (simple ${TARGET_PLATFORM} ${SRC})
target_compile_definitions (simple PRIVATE ${PLATFORM_DEFINES})

# TODO: add more libraries and stuff?

# where to output
install (TARGETS simple DESTINATION bin)

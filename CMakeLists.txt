# Refer to https://gist.github.com/mbinna/c61dbb39bca0e4fb7d1f73b0d66a4fd1 for best practices
cmake_minimum_required (VERSION 3.0)

project (GraphicsFramework)

# some platform independent consts
set (SRC_DIR src)
set (INC_DIR inc)
set (TESTS_SRC_DIR ${SRC_DIR}/tests)
set (TESTS_INC_DIR ${INC_DIR}/tests)
set (EXTERN_INC_DIR external/inc)

if (WIN32)
    set (TARGET_PLATFORM WIN32)

    set (PLATFORM_DEFINES -DWIN32 -D_WINDOWS -D_UNICODE -DUNICODE)

	# folder for platform specific libs
	set (PLATFORM_LIBS_DIR ${CMAKE_SOURCE_DIR}/external/precompiled/win32/lib)

    # TODO: add lib based on debug/release
    set (PLATFORM_LIBS
        ${PLATFORM_LIBS_DIR}/release/jobxx.lib
    )

	set (PLATFORM_LIBS_DEBUG
		${PLATFORM_LIBS_DIR}/debug/jobxx.lib
	)
endif()

# Main Project
set (ENTRYPOINTS_SRC 
    ${SRC_DIR}/main_win32.cpp
)
source_group ("entrypoints" FILES ${ENTRYPOINTS_SRC})

set (SRC ${ENTRYPOINTS_SRC})

set (SIMPLE_DEFINES ${PLATFORM_DEFINES})
set (SIMPLE_INCLUDES ${INC_DIR} ${EXTERN_INC_DIR})
set (SIMPLE_LIBS ${PLATFORM_LIBS})
set (SIMPLE_LIBS_DEBUG ${PLATFORM_LIBS_DEBUG})

# TODO: this shouldn't be a target, but it's probably useful for testing
add_executable (simple ${TARGET_PLATFORM} ${SRC})
target_compile_definitions (simple PRIVATE ${SIMPLE_DEFINES})
target_include_directories(simple PRIVATE ${SIMPLE_INCLUDES})
target_link_libraries (simple PRIVATE optimized ${SIMPLE_LIBS})
target_link_libraries (simple PRIVATE debug ${SIMPLE_LIBS_DEBUG})

# Tests executable (for unit testing)
set (TESTS_JOBXX_SRC
	${TESTS_SRC_DIR}/jobxx_basic.cpp
)
source_group("jobxx" FILES ${TESTS_JOBXX_SRC})

set (TESTS_CATCH_SRC
	${TESTS_SRC_DIR}/catch_basic.cpp
)
source_group("catch" FILES ${TESTS_CATCH_SRC})

set (TESTS_SRC ${TESTS_JOBXX_SRC} ${TESTS_CATCH_SRC})

set (TESTS_LIBS ${PLATFORM_LIBS})
set (TESTS_INCLUDES ${INC_DIR} ${EXTERN_INC_DIR})
set (TESTS_LIBS_DEBUG ${PLATFORM_LIBS_DEBUG})

add_executable (tests ${TESTS_SRC})
target_include_directories(tests PRIVATE ${TESTS_INCLUDES})
target_link_libraries(tests PRIVATE optimized ${TESTS_LIBS})
target_link_libraries(tests PRIVATE debug ${TESTS_LIBS_DEBUG})

# TODO: add more libraries and stuff?

# where to output
install (TARGETS simple tests DESTINATION bin)

# Refer to https://gist.github.com/mbinna/c61dbb39bca0e4fb7d1f73b0d66a4fd1 for best practices
cmake_minimum_required (VERSION 3.0)

project (GraphicsFramework)

# cache variables
set (BUILD_SHARED false CACHE BOOL "Build shared libraries as shared")
set (INCLUDE_EXTERNAL_SRC true CACHE BOOL "Include external library source in project")

# some platform independent consts
set (SRC_DIR src)
set (INC_DIR inc)
set (TESTS_SRC_DIR ${SRC_DIR}/tests)
set (TESTS_INC_DIR ${INC_DIR}/tests)
set (EXTERN_DIR external)
set (EXTERN_INC_DIR ${EXTERN_DIR}/inc)

if (WIN32)
    set (TARGET_PLATFORM WIN32)

    set (PLATFORM_DEFINES -DWIN32 -D_WINDOWS -D_UNICODE -DUNICODE)

	# folder for platform specific libs
	set (PLATFORM_LIBS_DIR ${CMAKE_SOURCE_DIR}/external/precompiled/win32/lib)

    set (PLATFORM_LIBS
        optimized ${PLATFORM_LIBS_DIR}/release/jobxx.lib
        debug ${PLATFORM_LIBS_DIR}/debug/jobxx.lib
    )

	if (MSVC)
		# enable support for solution folders
		set_property (GLOBAL PROPERTY USE_FOLDERS ON)
	endif()
endif()

# External projects
if (INCLUDE_EXTERNAL_SRC)
	set (JOBXX_DIR ${EXTERN_DIR}/jobxx)
	file (GLOB JOBXX_SRC ${JOBXX_DIR}/source/*.cc)
	file (GLOB JOBXX_INCLUDES ${JOBXX_DIR}/include/jobxx/*.h)
	add_library (jobxx STATIC EXCLUDE_FROM_ALL ${JOBXX_SRC} ${JOBXX_INCLUDES})
	target_include_directories (jobxx PRIVATE ${JOBXX_DIR}/include)
	if (MSVC)
		set_target_properties (jobxx PROPERTIES FOLDER external)
	endif()
endif()

if (BUILD_SHARED)
	set (SHARED_DEFINES -DGF_SHARED)
	set (SHARED_EXPORT_DEFINES -DGF_SHARED_EXPORT)
	set (SHARED_LIBRARY_TYPE SHARED)
else()
	set (SHARED_DEFINES)
	set (SHARED_LIBRARY_TYPE STATIC)
endif()

# Main Project
set (GRAPHICSFRAMEWORK_SRC_DIR ${SRC_DIR}/graphicsframework)
set (GRAPHICSFRAMEWORK_INC_DIR ${INC_DIR}/graphicsframework)

set (ENTRYPOINTS_SRC 
    ${GRAPHICSFRAMEWORK_INC_DIR}/main_win32.h
	${GRAPHICSFRAMEWORK_INC_DIR}/EntryPoint.h
)
source_group ("EntryPoints" FILES ${ENTRYPOINTS_SRC})

set (CORE_SRC
    ${GRAPHICSFRAMEWORK_INC_DIR}/Application.h
	${GRAPHICSFRAMEWORK_SRC_DIR}/Application.cpp
	${GRAPHICSFRAMEWORK_INC_DIR}/Config.h
)
source_group ("Core" FILES ${CORE_SRC})

set (SRC ${ENTRYPOINTS_SRC} ${CORE_SRC})

set (GRAPHICSFRAMEWORK_DEFINES ${PLATFORM_DEFINES} ${SHARED_DEFINES} ${SHARED_EXPORT_DEFINES})
set (GRAPHICSFRAMEWORK_INCLUDES ${INC_DIR} ${EXTERN_INC_DIR})
set (GRAPHICSFRAMEWORK_LIBS ${PLATFORM_LIBS})

add_library (graphicsframework ${SHARED_LIBRARY_TYPE} ${SRC})
target_compile_definitions (graphicsframework PRIVATE ${GRAPHICSFRAMEWORK_DEFINES})
target_include_directories(graphicsframework PRIVATE ${GRAPHICSFRAMEWORK_INCLUDES})
target_link_libraries (graphicsframework PRIVATE ${GRAPHICSFRAMEWORK_LIBS})

set (SIMPLE_SRC_DIR ${SRC_DIR}/samples)
set (SIMPLE_INC_DIR ${INC_DIR}/samples)

set (SIMPLE_SRC ${SIMPLE_SRC_DIR}/simple.cpp)

set (SIMPLE_DEFINES ${PLATFORM_DEFINES} ${SHARED_DEFINES})
set (SIMPLE_INCLUDES ${INC_DIR} ${EXTERN_INC_DIR})
set (SIMPLE_LIBS ${PLATFORM_LIBS})

# TODO: this shouldn't be a target, but it's probably useful for testing
add_executable (simple ${TARGET_PLATFORM} ${SIMPLE_SRC})
target_compile_definitions (simple PRIVATE ${SIMPLE_DEFINES})
target_include_directories(simple PRIVATE ${SIMPLE_INCLUDES})
target_link_libraries (simple PRIVATE graphicsframework ${SIMPLE_LIBS})

# TODO: add more libraries and stuff?

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

add_executable (tests ${TESTS_SRC})
target_include_directories(tests PRIVATE ${TESTS_INCLUDES})
target_link_libraries(tests PRIVATE graphicsframework ${TESTS_LIBS})

# Add tests and enable testing
enable_testing ()
add_test (NAME catch_tests COMMAND tests [catch])
add_test (NAME jobxx_tests COMMAND tests [jobxx])

# where to output
install (TARGETS simple tests DESTINATION bin)

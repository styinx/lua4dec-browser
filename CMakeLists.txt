# cmake -DVCPKG_MANIFEST_INSTALL:PATH="C:/Libs/vcpkg/installed" -DDIR_VCPKG_ROOT:PATH="C:/Libs/vcpkg/" -DVCPKG_TARGET_TRIPLET:STRING="x64-windows-static" -DCMAKE_TOOLCHAIN_FILE:PATH="C:/Libs/vcpkg/scripts/buildsystems/vcpkg.cmake" -Wno-dev

project(lua4dec-browser)

cmake_minimum_required(VERSION 3.21)

#
# Functions
#

# 'message' wrapper
function(print what)
    message("   ${what}")
endfunction()


FUNCTION(DEBUG_WX_LIBS var)
    SET(liblist "")
    FOREACH (f ${var})
        STRING(REGEX REPLACE "${DIR_VCPKG_LIBRARY}/wx(.*)u(.*)\\.lib" "${DIR_VCPKG_LIBRARY_DEBUG}/wx\\1ud\\2.lib" f ${f})
        LIST(APPEND liblist "${f}")
    ENDFOREACH ()
    SET(wxWidgets_LIBRARIES "${liblist}" PARENT_SCOPE)
ENDFUNCTION()


#
# Paths
#

# Local paths
set(DIR_ROOT "${PROJECT_SOURCE_DIR}")
set(DIR_INCLUDE "${DIR_ROOT}/include")
set(DIR_SOURCE "${DIR_ROOT}/source")
set(DIR_LIBRARY "${DIR_ROOT}/lib")
set(DIR_TEST "${DIR_ROOT}/test")

# VCPKG paths
set(DIR_VCPKG_INSTALL "${DIR_VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}")
set(DIR_VCPKG_INCLUDE "${DIR_VCPKG_INSTALL}/include")
set(DIR_VCPKG_LIBRARY "${DIR_VCPKG_INSTALL}/lib")
set(DIR_VCPKG_LIBRARY_DEBUG "${DIR_VCPKG_INSTALL}/debug/lib")

# Binary output paths
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${DIR_LIBRARY}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${DIR_LIBRARY}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${DIR_ROOT}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${DIR_LIBRARY}/deb")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${DIR_LIBRARY}/deb")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${DIR_ROOT}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${DIR_LIBRARY}/rel")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${DIR_LIBRARY}/rel")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${DIR_ROOT}")


#
# CXX
#

set(CMAKE_CXX_STANDARD 17)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang") # Clang
    #
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU") # GCC
    set(COMPILER_FLAGS "-Wall -Wextra -ansi -pedantic -Wno-format -std=c++17")
    set(COMPILER_FLAGS_DEBUG "-MTd")
    set(COMPILER_FLAGS_RELEASE "-O3 -DNDEBUG -MT")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC") # MSVC
    set(COMPILER_FLAGS "")
    set(COMPILER_FLAGS_DEBUG "/Zi /Ob0 /Od /RTC1 /MTd")
    set(COMPILER_FLAGS_RELEASE "/O2 /Ob2 /DNDEBUG /MT")
endif ()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${COMPILER_FLAGS} ${COMPILER_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${COMPILER_FLAGS} ${COMPILER_FLAGS_RELEASE}")

add_compile_definitions(_CRT_SECURE_NO_WARNINGS)


# Dependencies"

SET(wxWidgets_USE_LIBS base core propgrid stc)
SET(wxWidgets_FIND_COMPONENTS true)
find_package(wxWidgets REQUIRED)

IF ("${CMAKE_BUILD_TYPE}" MATCHES "Debug")
    DEBUG_WX_LIBS("${wxWidgets_LIBRARIES}")
ELSE ()
ENDIF ()


# Local Dependencies

add_subdirectory(lua4dec)


# CMAKE

set(INCLUDE_DIRECTORIES
        ${DIR_INCLUDE}
        ${DIR_SOURCE}

        lua4dec/source/

        ${DIR_VCPKG_INCLUDE})

include_directories(${INCLUDE_DIRECTORIES})

if (CMAKE_BUILD_TYPE EQUAL "DEBUG")
    set(LIBRARY_DIRECTORIES
            ${DIR_VCPKG_LIBRARY}
            ${DIR_VCPKG_LIBRARY}/deb)
else()
    set(LIBRARY_DIRECTORIES
            ${DIR_VCPKG_LIBRARY}
            ${DIR_VCPKG_LIBRARY}/rel)
endif()

link_directories(${LIBRARY_DIRECTORIES})


# Sources

set(SOURCES
    source/app.cpp
)

source_group("source"         FILES
    source/main.cpp
    source/app.hpp
    source/app.cpp)



# Binaries

add_executable(Main source/main.cpp ${SOURCES})
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # Build without console
    set_target_properties(Main PROPERTIES LINK_FLAGS /SUBSYSTEM:WINDOWS)
    set_target_properties(Main PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif ()
target_link_libraries(Main PRIVATE ${wxWidgets_LIBRARIES} wxscintilla)


project (SpaceMath CXX)

cmake_minimum_required(VERSION 3.3.1)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(SPACEMATH_BUILD_TEST "Build test executables" ON)

option(USE_MSVC_RUNTIME_LIBRARY_DLL "Use MSVC runtime library DLL" ON)

#--------------------------------------------------------------------
# Set compiler specific flags
#--------------------------------------------------------------------
if (MSVC)
    if (NOT USE_MSVC_RUNTIME_LIBRARY_DLL)
        foreach (flag CMAKE_CXX_FLAGS
                      CMAKE_CXX_FLAGS_DEBUG
                      CMAKE_CXX_FLAGS_RELEASE
                      CMAKE_CXX_FLAGS_MINSIZEREL
                      CMAKE_CXX_FLAGS_RELWITHDEBINFO)

            if (${flag} MATCHES "/MD")
                string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
            endif()
            if (${flag} MATCHES "/MDd")
                string(REGEX REPLACE "/MDd" "/MTd" ${flag} "${${flag}}")
            endif()
        endforeach()
    endif()
endif()

#--------------------------------------------------------------------
# Add subdirectories and set Project
#--------------------------------------------------------------------
add_library(spacemath "${CMAKE_CURRENT_SOURCE_DIR}/include/spacemath.h")
set_target_properties(spacemath PROPERTIES LINKER_LANGUAGE CXX)

if (SPACEMATH_BUILD_TEST)
    #add_subdirectory(tests)
endif()
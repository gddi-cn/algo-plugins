cmake_minimum_required(VERSION 3.14.0)
project(gddi-plugins VERSION 1.0.0 LANGUAGES CXX C)

include(FetchContent)
include(ExternalProject)

if(MSVC)
    set(CMAKE_CXX_STANDARD 20)
else()
    set(CMAKE_CXX_STANDARD 17)
endif()

set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)
find_package(Threads REQUIRED)

set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external/${CMAKE_SYSTEM_PROCESSOR})
include_directories(${EXTERNAL_INSTALL_LOCATION}/include)
link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)
list(APPEND CMAKE_FIND_ROOT_PATH ${EXTERNAL_INSTALL_LOCATION})
set(ENV{PKG_CONFIG_PATH} ${EXTERNAL_INSTALL_LOCATION}/share/pkgconfig:$ENV{PKG_CONFIG_PATH})
set(ENV{PKG_CONFIG_PATH} ${EXTERNAL_INSTALL_LOCATION}/lib/pkgconfig:$ENV{PKG_CONFIG_PATH})

file(DOWNLOAD https://github.com/nlohmann/json/releases/download/v3.10.5/json.hpp
    ${EXTERNAL_INSTALL_LOCATION}/include/json.hpp
    EXPECTED_MD5 f62f0c62d5504e587f57acb4eb3cea30
)

file(DOWNLOAD "https://github.com/gddi-cn/archive/raw/refs/heads/main/plugin-core/include/abstract_plugin.h"
    ${EXTERNAL_INSTALL_LOCATION}/include/abstract_plugin.h
    EXPECTED_MD5 3f5cf9fd467fd16e72e81b5d7f9d5cee
)

if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
    file(DOWNLOAD "https://github.com/gddi-cn/archive/raw/refs/heads/main/plugin-core/lib/x86_64/libplugin_core.so.1.0.0"
        ${EXTERNAL_INSTALL_LOCATION}/lib/libplugin_core.so.1.0.0
        EXPECTED_MD5 92026cbf7f826c255f4ab152bc0a966c
    )
    file(CREATE_LINK ${EXTERNAL_INSTALL_LOCATION}/lib/libplugin_core.so.1.0.0 ${EXTERNAL_INSTALL_LOCATION}/lib/libplugin_core.so.1 SYMBOLIC)
    file(CREATE_LINK ${EXTERNAL_INSTALL_LOCATION}/lib/libplugin_core.so.1.0.0 ${EXTERNAL_INSTALL_LOCATION}/lib/libplugin_core.so SYMBOLIC)
endif()

file(GLOB FOLDERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/plugins/*)
foreach(FOLDER ${FOLDERS})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${FOLDER})
        if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${FOLDER}/CMakeLists.txt)
            message(STATUS "Adding subdirectory: ${FOLDER}")
            add_subdirectory(${FOLDER})
        endif()
    endif()
endforeach()

set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/release")
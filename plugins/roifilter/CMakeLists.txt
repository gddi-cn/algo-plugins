pkg_check_modules(Clipper2 QUIET IMPORTED_TARGET Clipper2)

if (Clipper2_FOUND)
    message(STATUS "Found Clipper2: ${clipper2_CONFIG} (found version \"${Clipper2_VERSION}\")")
else()
    ExternalProject_Add(
        Clipper2_external
        GIT_REPOSITORY http://git.mirror.gddi.io/mirror/Clipper2.git
        GIT_TAG Clipper2_1.0.6
        GIT_SHALLOW TRUE
        PREFIX ${CMAKE_BINARY_DIR}/external/${CMAKE_SYSTEM_PROCESSOR}
        BUILD_IN_SOURCE 1
        CMAKE_ARGS -S./CPP -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/external/${CMAKE_SYSTEM_PROCESSOR} -DCMAKE_POSITION_INDEPENDENT_CODE=ON
                -DCMAKE_SYSTEM_PROCESSOR=${CMAKE_SYSTEM_PROCESSOR} -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} -DCLIPPER2_TESTS=OFF
        BUILD_BYPRODUCTS ${CMAKE_BINARY_DIR}/external/${CMAKE_SYSTEM_PROCESSOR}/lib/libClipper2.a
    )

    add_library(Clipper2 INTERFACE IMPORTED)
    add_dependencies(Clipper2 Clipper2_external)
endif()

file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c??")
add_library(gddi_roifilter SHARED ${SRC_FILES})
target_link_libraries(gddi_roifilter plugin_core Clipper2)
set_target_properties(gddi_roifilter PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION 1)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    install(TARGETS gddi_roifilter RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
endif()
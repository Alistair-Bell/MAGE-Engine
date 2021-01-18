set(${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
cmake_minimum_required(VERSION 3.7.0)

project("MageEngine")

option(PROJECT_BUILD_PLATFORM "Engine build platform target" "Windows")
option(PROJECT_BUILD_UNIT_TESTS "Build engine unit tests" On)
option(PROJECT_BUILD_LINUX_DISPLAY_BACKEND "Linux display server backend" "X11")

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)

set(PACKAGE_NAME "${PROJECT_NAME}")
set(PACKAGE_TARNAME "mage-engine")
set(PACKAGE_VERSION "${PROJECT_VERSION}")
set(PACKAGE_STRING "${PROJECT_NAME} ${PROJECT_VERSION}")
set(PACKAGE_LICENSE "GNU General Public License, version 3")

if (PROJECT_BUILD_PLATFORM STREQUAL "Windows")
    set(PROJECT_BUILD_LINUX_DISPLAY_BACKEND "Off")
endif()

message(STATUS "${PROJECT_BUILD_LINUX_DISPLAY_BACKEND}")

if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else()
    add_compile_options("-Wall" "-std=c99")
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options("-Werror")
    endif()
endif(MSVC)

find_package(Vulkan)

function(MageEngineAddFlagsAndLink binary)

    if (PROJECT_BUILD_PLATFORM STREQUAL "Linux")
        target_compile_definitions(${binary} PUBLIC MAGE_BUILD_PLATFORM_LINUX=1)
        target_link_libraries(${binary} pthread)
    elseif (PROJECT_BUILD_PLATFORM STREQUAL "Windows")
		target_compile_definitions(${binary} PUBLIC MAGE_BUILD_PLATFORM_WINDOWS=1)
    endif()

    if (PROJECT_BUILD_LINUX_DISPLAY_BACKEND STREQUAL "X11")
        target_compile_definitions(${binary} PUBLIC MAGE_BUILD_XLIB=1)
        target_link_libraries(${binary} X11)
    elseif(PROJECT_BUILD_LINUX_DISPLAY_BACKEND STREQUAL "Wayland")
        target_compile_definitions(${binary} PUBLIC MAGE_BUILD_WAYLAND=1)
        target_link_libraries(${binary} wayland-client)
        target_link_libraries(${binary} wayland-server)
    endif()
    
    target_compile_options(${binary} PUBLIC "-g")

    if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	    target_compile_definitions(${binary} PUBLIC MAGE_BUILD_DEBUG_MODE=1)
    else()
        target_compile_options(${binary} PUBLIC "-O2")
        target_compile_definitions(${binary} PUBLIC MAGE_BUILD_DEBUG_MODE=0)
    endif()

	target_link_libraries(${binary} Vulkan::Vulkan)
    target_include_directories(${binary} PUBLIC ${Vulkan_INCLUDE_DIRS})
    target_compile_definitions(${binary} PUBLIC MAGE_BUILD_RENDERER_VULKAN=1)
    
endfunction(MageEngineAddFlagsAndLink binary)

function(MageEngineAddUnitTests binary source)
    add_executable(${binary} ${source})
	target_link_libraries(${binary} MageSDK)
    add_test(NAME ${binary} COMMAND ${binary})
    MageEngineAddFlagsAndLink(${binary})
endfunction(MageEngineAddUnitTests binary source)


add_subdirectory(Mage)
add_subdirectory(Sandbox)

if (PROJECT_BUILD_UNIT_TESTS)
    enable_testing()
    message(STATUS "Building unit tests!")
    add_subdirectory(Tests)
endif()

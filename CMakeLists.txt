set(${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
cmake_minimum_required(VERSION 3.7.0)

project("MageEngine")

option(PROJECT_BUILD_PLATFORM "Engine build platform target" "Windows")
option(PROJECT_BUILD_UNIT_TESTS "Build engine unit tests" On)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)

set(PACKAGE_NAME "${PROJECT_NAME}")
set(PACKAGE_TARNAME "mage-enigne")
set(PACKAGE_VERSION "${PROJECT_VERSION}")
set(PACKAGE_STRING "${PROJECT_NAME} ${PROJECT_VERSION}")
set(PACKAGE_LICENSE "GNU General Public License, version 3")

if (MSVC)
    add_compile_options()
else()
    add_compile_options("-Wall" "-std=c99" "-Werror")
endif(MSVC)

find_package(Vulkan)

function(MageEngineAddDefinesAndFlagsToBinary binary)
    
    if (PROJECT_BUILD_PLATFORM STREQUAL "Linux")
        target_compile_definitions(${binary} PUBLIC MAGE_BUILD_PLATFORM_LINUX=1)
    elseif (PROJECT_BUILD_PLATFORM STREQUAL "Windows")
		target_compile_definitions(${binary} PUBLIC MAGE_BUILD_PLATFORM_WINDOWS=1)
    endif()
    
    target_compile_options(${binary} PUBLIC "-g")

    if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	    target_compile_definitions(${binary} PUBLIC MAGE_BUILD_DEBUG_MODE=1)
    else()
        target_compile_options(${binary} PUBLIC "-O2")
        target_compile_definitions(${binary} PUBLIC MAGE_BUILD_DEBUG_MODE=0)
    endif()

    target_compile_definitions(${binary} PUBLIC MAGE_BUILD_RENDERER_VULKAN=1)
    
endfunction(MageEngineAddDefinesAndFlagsToBinary binary)

function(MageEngineLinkPlatformLibrariesToBinary binary)

    if (PROJECT_BUILD_PLATFORM STREQUAL "Linux")
        target_link_libraries(${binary} X11)
    elseif (PROJECT_BUILD_PLATFORM STREQUAL "Windows")
    
    endif()

    target_link_libraries(${binary} Vulkan::Vulkan)
    target_include_directories(${binary} PUBLIC ${Vulkan_INCLUDE_DIRS})
    
endfunction(MageEngineLinkPlatformLibrariesToBinary)

function(MageEngineAddUnitTests binary source)
    
    add_executable(${binary} ${source})
	target_link_libraries(${binary} MageSDK)
    add_test(NAME ${binary} COMMAND ${binary})
    MageEngineLinkPlatformLibrariesToBinary(${binary})
    MageEngineAddDefinesAndFlagsToBinary(${binary})

endfunction(MageEngineAddUnitTests binary source)


add_subdirectory(Mage)
add_subdirectory(Sandbox)

if (PROJECT_BUILD_UNIT_TESTS)
    enable_testing()
    message(STATUS "Building unit tests!")
    add_subdirectory(Tests)
endif()

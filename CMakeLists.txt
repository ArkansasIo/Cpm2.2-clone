cmake_minimum_required(VERSION 3.20)

project(CPMUniversalDevelopmentEnvironment
    VERSION 0.1.0
    DESCRIPTION "Clean-room CP/M-style emulation components"
    LANGUAGES CXX
)

include(CTest)
option(CPM22_ENABLE_SDL3 "Build the optional SDL3 frontend" OFF)

add_library(cpm22_core
    src/audio/retro_audio.cpp
    src/audio/storage_audio.cpp
    src/devices/storage.cpp
    src/platform/architecture.cpp
    src/platform/cmos_rtc.cpp
    src/platform/linux_kernel_layer.cpp
    src/gui/linux_gui.cpp
    src/cpu/cpu8080.cpp
)

target_compile_features(cpm22_core PUBLIC cxx_std_23)
target_include_directories(cpm22_core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

if(MSVC)
    target_compile_options(cpm22_core PRIVATE /W4 /permissive-)
else()
    target_compile_options(cpm22_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(CPM22_ENABLE_SDL3)
    find_package(SDL3 CONFIG REQUIRED)
    target_sources(cpm22_core PRIVATE src/gui/sdl3_gui.cpp)
    target_compile_definitions(cpm22_core PUBLIC CPM22_ENABLE_SDL3=1)
    target_link_libraries(cpm22_core PUBLIC SDL3::SDL3)
endif()

if(BUILD_TESTING)
    add_executable(test_cpm_audio tests/test_cpm_audio.cpp)
    target_link_libraries(test_cpm_audio PRIVATE cpm22_core)
    add_test(NAME cpm_audio_integration COMMAND test_cpm_audio)

    add_executable(test_cpu8080 tests/test_cpu8080.cpp)
    target_link_libraries(test_cpu8080 PRIVATE cpm22_core)
    add_test(NAME cpu8080_regression COMMAND test_cpu8080)
endif()

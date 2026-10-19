cmake_minimum_required(VERSION 3.20)

project(CPMUniversalDevelopmentEnvironment
    VERSION 0.3.0
    DESCRIPTION "Clean-room CP/M 2.2-style emulator and retro-computing development environment"
    LANGUAGES CXX
)

include(CTest)
option(CPM22_ENABLE_SDL3 "Build the optional SDL3 frontend" OFF)
option(CPM22_BUILD_APP "Build the command-line emulator application" ON)

add_library(cpm22_core
    src/audio/retro_audio.cpp
    src/audio/storage_audio.cpp
    src/devices/storage.cpp
    src/platform/architecture.cpp
    src/platform/cmos_rtc.cpp
    src/platform/linux_kernel_layer.cpp
    src/gui/linux_gui.cpp
    src/cpu/cpu8080.cpp
    src/cpm/cpm_machine.cpp
    src/cpm/console.cpp
    src/cpm/directory.cpp
    src/cpm/bdos.cpp
    src/cpm/bios.cpp
)

target_compile_features(cpm22_core PUBLIC cxx_std_23)
target_include_directories(cpm22_core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

target_compile_definitions(cpm22_core PUBLIC CPM22_VERSION_MAJOR=0 CPM22_VERSION_MINOR=3)

if(MSVC)
    target_compile_options(cpm22_core PRIVATE /W4 /permissive-)
else()
    target_compile_options(cpm22_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(CPM22_BUILD_APP)
    add_executable(cpm22 apps/cpm22_main.cpp)
    target_link_libraries(cpm22 PRIVATE cpm22_core)
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

    add_executable(test_cpm_directory tests/test_cpm_directory.cpp)
    target_link_libraries(test_cpm_directory PRIVATE cpm22_core)
    add_test(NAME cpm_directory COMMAND test_cpm_directory)

    add_executable(test_cpm_bdos tests/test_cpm_bdos.cpp)
    target_link_libraries(test_cpm_bdos PRIVATE cpm22_core)
    add_test(NAME cpm_bdos_services COMMAND test_cpm_bdos)
endif()

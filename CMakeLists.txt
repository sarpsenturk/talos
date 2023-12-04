cmake_minimum_required(VERSION 3.13)

option(TALOS_DEV_MODE "Enable developer mode for Talos" ON)
option(TALOS_TESTING "Enable tests for Talos" ${TALOS_DEV_MODE})

if (TALOS_TESTING)
    list(APPEND VCPKG_MANIFEST_FEATURES "tests")
endif ()

project(talos VERSION 0.1.0 LANGUAGES CXX C)

add_library(talos_lib "")
target_compile_features(talos_lib PUBLIC cxx_std_20)
target_include_directories(talos_lib PUBLIC src)

add_executable(talos_exe src/main.cpp)
target_link_libraries(talos_exe talos_lib)

add_subdirectory(src)

if (TALOS_TESTING)
    enable_testing()
    add_subdirectory(test)
endif ()

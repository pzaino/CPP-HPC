cmake_minimum_required(VERSION 3.10)

# Project Name
project(CPP-HPC)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Option to enable recursive form
option(USE_RECURSIVE_FORM "Enable recursive form of algorithms" OFF)

# Option to enable testing
option(ENABLE_TESTING "Enable testing" OFF)

# Ensure the correct compiler is used for macOS with OpenMP
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    if (CMAKE_SYSTEM_PROCESSOR MATCHES "arm")
        # macOS ARM (Apple Silicon)
        set(LLVM_DIR "/opt/homebrew/opt/llvm")
    else()
        # macOS x86_64
        set(LLVM_DIR "/usr/local/opt/llvm")
    endif()
    include_directories(${LLVM_DIR}/include)
    link_directories(${LLVM_DIR}/lib)
    set(CMAKE_C_COMPILER "${LLVM_DIR}/bin/clang")
    set(CMAKE_CXX_COMPILER "${LLVM_DIR}/bin/clang++")
    set(OpenMP_CXX_FLAGS "-Xpreprocessor -fopenmp -I${LLVM_DIR}/include")
    set(OpenMP_CXX_LIB_NAMES "omp")
    set(OpenMP_omp_LIBRARY "${LLVM_DIR}/lib/libomp.dylib")
endif()

# Function to collect all source files and create targets
function(create_targets_from_directory dir)
    file(GLOB_RECURSE sources "${dir}/*.cpp")
    foreach(source ${sources})
        get_filename_component(target_name ${source} NAME_WE)

        # Create shared library
        add_library(${target_name}_shared SHARED ${source})

        # Create static library
        add_library(${target_name}_static STATIC ${source})

        # Optionally link libraries if necessary
        if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
            find_library(LIBUNWIND libunwind.1.dylib PATHS ${LLVM_DIR}/lib)
            if(LIBUNWIND)
                target_link_libraries(${target_name}_shared PRIVATE ${LIBUNWIND})
                target_link_libraries(${target_name}_static PRIVATE ${LIBUNWIND})
            endif()
        elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
            find_package(OpenMP REQUIRED)
            if(OPENMP_FOUND)
                target_link_libraries(${target_name}_shared PRIVATE OpenMP::OpenMP_CXX)
                target_link_libraries(${target_name}_static PRIVATE OpenMP::OpenMP_CXX)
            endif()
        elseif(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
            find_package(OpenMP REQUIRED)
            if(OPENMP_FOUND)
                target_link_libraries(${target_name}_shared PRIVATE OpenMP::OpenMP_CXX)
                target_link_libraries(${target_name}_static PRIVATE OpenMP::OpenMP_CXX)
            endif()
        else()
            message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}")
        endif()

        # Add the definition if USE_RECURSIVE_FORM is ON
        if(USE_RECURSIVE_FORM)
            target_compile_definitions(${target_name}_shared PRIVATE ZFP_RECURSIVE_FORM)
            target_compile_definitions(${target_name}_static PRIVATE ZFP_RECURSIVE_FORM)
        endif()
    endforeach()
endfunction()

# Include directories
include_directories(OpenMP CUDA OpenCL SIMD)

# Create targets from each directory
create_targets_from_directory(${CMAKE_SOURCE_DIR}/OpenMP)
create_targets_from_directory(${CMAKE_SOURCE_DIR}/CUDA)
create_targets_from_directory(${CMAKE_SOURCE_DIR}/OpenCL)
create_targets_from_directory(${CMAKE_SOURCE_DIR}/SIMD)

# Enable testing if the option is set
if(ENABLE_TESTING)
    enable_testing()

    # Include GoogleTest
    find_package(GTest REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})

    # Function to add test for a library
    function(add_test_for_library lib_name)
        add_executable(${lib_name}_test ../tests/${lib_name}_test.cpp)
        target_link_libraries(${lib_name}_test ${lib_name}_shared ${GTEST_LIBRARIES} pthread)
        add_test(NAME ${lib_name}_test COMMAND ${lib_name}_test)
    endfunction()

    # Add tests
    add_test_for_library(omp_dijkstra)
    add_test_for_library(omp_matrix_multipy)
    add_test_for_library(omp_mergesort)
    add_test_for_library(omp_quicksort)
    add_test_for_library(omp_sum_sub)
    add_test_for_library(omp_fft)
endif()

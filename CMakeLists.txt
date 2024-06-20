cmake_minimum_required(VERSION 3.10)

# Project Name
project(CPP-HPC)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add options to enable/disable different libraries
option(ENABLE_OPENMP "Enable OpenMP support" ON)
option(ENABLE_OPENCL "Enable OpenCL support" ON)
option(ENABLE_CUDA "Enable CUDA support" OFF)
option(ENABLE_SIMD "Enable SIMD support" OFF)
option(ENABLE_TESTING "Enable Testing" OFF)

# Include directories and link libraries based on the options
include_directories(${CMAKE_SOURCE_DIR}/includes)

if (ENABLE_OPENMP)
    find_package(OpenMP)
    if(OpenMP_FOUND)
        add_compile_options(${OpenMP_CXX_FLAGS})
        set(OpenMP_LIBS OpenMP::OpenMP_CXX)
    else()
        if(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
            message(STATUS "Using AppleClang for OpenMP")
            set(OpenMP_CXX_FLAGS "-Xpreprocessor -fopenmp -I/opt/homebrew/opt/llvm/include")
            set(OpenMP_LIBS "/opt/homebrew/opt/llvm/lib/libomp.dylib")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
        else()
            message(FATAL_ERROR "OpenMP not found and not using AppleClang")
        endif()
    endif()
endif()

if (ENABLE_OPENCL)
    find_package(OpenCL REQUIRED)
    if(OpenCL_FOUND)
        include_directories(${OpenCL_INCLUDE_DIRS})

        # Add architecture-specific paths for OpenCL headers
        if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
            if (CMAKE_SYSTEM_PROCESSOR MATCHES "arm")
                # macOS ARM (Apple Silicon)
                include_directories("/opt/homebrew/opt/opencl-clhpp-headers/include")
            else()
                # macOS x86_64
                include_directories("/usr/local/opt/opencl-clhpp-headers/include")
            endif()
        endif()

        # Define the target OpenCL version
        add_definitions(-DCL_HPP_TARGET_OPENCL_VERSION=120)
        add_definitions(-DCL_HPP_MINIMUM_OPENCL_VERSION=120)

        set(OpenCL_LIBS ${OpenCL_LIBRARIES})
    else()
        message(FATAL_ERROR "OpenCL requested but not found")
    endif()
endif()

if (ENABLE_CUDA)
    find_package(CUDA)
    if(CUDA_FOUND)
        include_directories(${CUDA_INCLUDE_DIRS})
        set(CUDA_LIBS ${CUDA_LIBRARIES})
    else()
        message(FATAL_ERROR "CUDA requested but not found")
    endif()
endif()

if (ENABLE_SIMD)
    # SIMD-specific settings
    add_compile_options(-march=native)
    set(SIMD_LIBS "")
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
        if (TARGET ${target_name}_shared)
            if (ENABLE_OPENMP)
                target_link_libraries(${target_name}_shared PRIVATE ${OpenMP_LIBS})
            endif()
            if (ENABLE_OPENCL)
                target_link_libraries(${target_name}_shared PRIVATE ${OpenCL_LIBS})
            endif()
            if (ENABLE_CUDA)
                target_link_libraries(${target_name}_shared PRIVATE ${CUDA_LIBS})
            endif()
            if (ENABLE_SIMD)
                target_link_libraries(${target_name}_shared PRIVATE ${SIMD_LIBS})
            endif()
        endif()

        if (TARGET ${target_name}_static)
            if (ENABLE_OPENMP)
                target_link_libraries(${target_name}_static PRIVATE ${OpenMP_LIBS})
            endif()
            if (ENABLE_OPENCL)
                target_link_libraries(${target_name}_static PRIVATE ${OpenCL_LIBS})
            endif()
            if (ENABLE_CUDA)
                target_link_libraries(${target_name}_static PRIVATE ${CUDA_LIBS})
            endif()
            if (ENABLE_SIMD)
                target_link_libraries(${target_name}_static PRIVATE ${SIMD_LIBS})
            endif()
        endif()
    endforeach()
endfunction()

# Include directories and create targets based on the enabled options
if (ENABLE_OPENMP)
    include_directories(OpenMP)
    create_targets_from_directory(${CMAKE_SOURCE_DIR}/OpenMP)
endif()

if (ENABLE_OPENCL)
    include_directories(OpenCL)
    create_targets_from_directory(${CMAKE_SOURCE_DIR}/OpenCL)
endif()

if (ENABLE_CUDA)
    include_directories(CUDA)
    create_targets_from_directory(${CMAKE_SOURCE_DIR}/CUDA)
endif()

if (ENABLE_SIMD)
    include_directories(SIMD)
    create_targets_from_directory(${CMAKE_SOURCE_DIR}/SIMD)
endif()

# Ensure the correct compiler is used for macOS with OpenMP
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin" AND ENABLE_OPENMP)
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
    find_library(LIBUNWIND libunwind.1.dylib PATHS ${LLVM_DIR}/lib)
    if(LIBUNWIND)
        foreach(source ${sources})
            get_filename_component(target_name ${source} NAME_WE)
            if (TARGET ${target_name}_shared)
                target_link_libraries(${target_name}_shared PRIVATE ${LIBUNWIND})
            endif()
            if (TARGET ${target_name}_static)
                target_link_libraries(${target_name}_static PRIVATE ${LIBUNWIND})
            endif()
        endforeach()
    endif()
endif()

# Add tests if enabled
if (ENABLE_TESTING)
    enable_testing()
    find_package(GTest REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})

    function(add_test_for_library library)
        get_filename_component(lib_name ${library} NAME_WE)
        add_executable(${lib_name}_test ../tests/${lib_name}_test.cpp)
        target_link_libraries(${lib_name}_test PRIVATE ${library}_shared ${GTEST_BOTH_LIBRARIES} pthread)
        if (ENABLE_OPENCL)
            target_link_libraries(${lib_name}_test PRIVATE ${OpenCL_LIBS})
        endif()
        add_test(NAME ${lib_name}_test COMMAND ${lib_name}_test)
    endfunction()

    if (ENABLE_OPENMP)
        add_test_for_library(omp_dijkstra)
        add_test_for_library(omp_matrix_multipy)
        add_test_for_library(omp_mergesort)
        add_test_for_library(omp_quicksort)
        add_test_for_library(omp_sum_sub)
        add_test_for_library(omp_fft)
        add_test_for_library(omp_conjugate_gradient)
        add_test_for_library(omp_k_means_clustering)
        add_test_for_library(omp_montecarlo_simulation)
    endif()

    if (ENABLE_OPENCL)
        add_test_for_library(ocl_dijkstra)
    endif()
endif()

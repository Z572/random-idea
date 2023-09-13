cmake_minimum_required(VERSION 3.7.1)

if(POLICY CMP0077)
  cmake_policy(SET CMP0077 NEW)
endif()

if(POLICY CMP0116)
  cmake_policy(SET CMP0116 OLD)
endif()
project(guile-mlir)

find_package(PkgConfig REQUIRED)
find_package(LLVM REQUIRED CONFIG)
find_package(MLIR REQUIRED CONFIG)
find_package(Schmutz REQUIRED)
pkg_check_modules(GUILE3 REQUIRED guile-3.0)
set(SOURCE_FILES guile_mlir.cpp)
include(TableGen)
include(AddLLVM)

message(STATUS "This is BINARY dir " ${PROJECT_BINARY_DIR})
message(STATUS "This is SOURCE dir " ${PROJECT_SOURCE_DIR})
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${MLIR_INCLUDE_DIRS})
include_directories(${MLIR_INCLUDE_DIRS})
message(STATUS "include path: ${LLVM_INCLUDE_DIRS}")
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Now build our tools
add_library(guile-mlir SHARED ${SOURCE_FILES})
include_directories(include)

include(HandleLLVMOptions)
# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs support core irreader orcjit native)

# Link against LLVM libraries
target_include_directories(guile-mlir PUBLIC ${LLVM_INCLUDE_DIRS} ${GUILE3_INCLUDE_DIRS}  ${MLIR_INCLUDE_DIRS})
target_link_libraries(guile-mlir PUBLIC ${llvm_libs} ${GUILE3_LIBRARIES}
    MLIRAnalysis
    MLIRIR
    MLIRParser
    MLIRSideEffectInterfaces
    MLIRTransforms)

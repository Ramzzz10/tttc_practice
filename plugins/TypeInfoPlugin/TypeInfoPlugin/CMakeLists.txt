cmake_minimum_required(VERSION 3.20)

project(TypeInfoPlugin)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(LLVM_DIR "/Users/midasxlr/Desktop/tttc_practice-main/build_rel/lib/cmake/llvm")
set(Clang_DIR "/Users/midasxlr/Desktop/tttc_practice-main/build_rel/lib/cmake/clang")

set(LLVM_INCLUDE_DIR "/Users/midasxlr/Desktop/tttc_practice-main/llvm/include")
set(CLANG_INCLUDE_DIR "/Users/midasxlr/Desktop/tttc_practice-main/clang/include")

include_directories(${LLVM_INCLUDE_DIR})
include_directories(${CLANG_INCLUDE_DIR})

include_directories("/Users/midasxlr/Desktop/tttc_practice-main/build_rel/include")
include_directories("/Users/midasxlr/Desktop/tttc_practice-main/build_rel/tools/clang/include")

find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

add_definitions(${LLVM_DEFINITIONS})

include(AddLLVM)

llvm_map_components_to_libnames(llvm_libs support core)

add_llvm_library(TypeInfoPlugin MODULE TypeInfoPlugin.cpp PLUGIN_TOOL clang PARTIAL_SOURCES_INTENDED)

target_link_libraries(TypeInfoPlugin PRIVATE ${llvm_libs} clangAST clangBasic clangFrontend)

add_llvm_library(FunctionInstrumenterPass SHARED FunctionInstrumenter.cpp PARTIAL_SOURCES_INTENDED)
target_link_libraries(FunctionInstrumenterPass PRIVATE ${llvm_libs})

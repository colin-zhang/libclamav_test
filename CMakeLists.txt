cmake_minimum_required(VERSION 3.5)
project(clamav_test)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS
  "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -g"
)

add_subdirectory(clamav-devel-clamav-0.100.0)

link_directories(opt/lib64)
link_directories(opt/lib)
include_directories(opt/include)

aux_source_directory(src SrcList)
add_executable(av_test ${SrcList})
#add_executable(av_test src/main.cpp src/clamav_engine.cpp)

target_link_libraries(av_test libclamav.a libclammspack.a libclamunrar.a libclamunrar_iface.a)
#target_link_libraries(av_test libclamav_0992.a)
target_link_libraries(av_test dl ltdl m pthread z pcre pcre2-8 json-c xml2 crypto ssl lzma)
target_link_libraries(av_test profiler)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

execute_process(COMMAND echo "finish")

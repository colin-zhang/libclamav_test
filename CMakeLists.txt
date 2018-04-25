cmake_minimum_required(VERSION 3.5)
project(av_test)

set(CMAKE_CXX_STANDARD 11)

link_directories(opt/lib64)
link_directories(opt/lib)
include_directories(opt/include)

aux_source_directory(src SrcList)
add_executable(av_test ${SrcList})
#add_executable(av_test src/main.cpp src/clamav_engine.cpp)

target_link_libraries(av_test libclamav.a libclammspack.a libclamunrar.a libclamunrar_iface.a)
#target_link_libraries(av_test libclamav_0992.a)
target_link_libraries(av_test dl ltdl m pthread z pcre xml2 crypto ssl lzma)
target_link_libraries(av_test profiler)

execute_process(COMMAND echo "finish")

# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)
# 项目信息
project (Tiny_STL)
# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)
# 添加 math 子目录
add_subdirectory(src)
# 指定生成目标 
add_executable(test ./test/test.cpp)
add_executable(test_vector ./test/test_vector.cpp)
add_executable(test_list ./test/test_list.cpp)
add_executable(test_rbtree ./test/test_rbtree.cpp)
add_executable(test_set ./test/test_set.cpp)
add_executable(test_queue ./test/test_queue.cpp)
add_executable(test_ptr ./test/test_ptr.cpp)
# 添加链接库
target_link_libraries(test srcs)
target_link_libraries(test_vector srcs)
target_link_libraries(test_list srcs)
target_link_libraries(test_rbtree srcs)
target_link_libraries(test_set srcs)
target_link_libraries(test_queue srcs)
target_link_libraries(test_ptr srcs)

#设置输出目录
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

#设置编译器及编译选项,cmake多线程应加上-pthread
SET(CMAKE_C_COMPILER g++)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -pthread ")
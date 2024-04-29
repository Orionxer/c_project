# CMake最低版本号要求
cmake_minimum_required(VERSION 3.0)

# 配置项目名
project(fsm)

# 设置包含目录
include_directories(include)

# 递归遍历获取项目的所有源文件列表
file(GLOB_RECURSE SRC_LIST FOLLOW_SYMLINKS main.c source/*.c)

# 生成可执行文件 main，后面是源码列表
add_executable(main ${SRC_LIST})

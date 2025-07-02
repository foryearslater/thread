# CMake最低版本要求
cmake_minimum_required(VERSION 3.21)

# 项目名称
project(MyProject)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB MODULE_DIRS "modules/*")
foreach(MODULE_DIR ${MODULE_DIRS})
    if(IS_DIRECTORY ${MODULE_DIR})
        add_subdirectory(${MODULE_DIR})
    endif()
endforeach()
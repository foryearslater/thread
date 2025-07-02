
# 创建可执行文件
add_executable(ThreadPassArg)

# 设置头文件目录
target_include_directories(ThreadPassArg PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)

# 搜索 src 文件夹下的所有源文件并添加到 SRC_LIST 变量中
file(GLOB SRC_LIST ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

target_sources(ThreadPassArg PRIVATE ${SRC_LIST})
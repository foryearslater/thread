project(App)

# 设置头文件目录
include_directories(${PROJECT_SOURCE_DIR}/include)

# 搜索 src 文件夹下的所有源文件并添加到 SRC_LIST 变量中
file(GLOB SRC_LIST ${PROJECT_SOURCE_DIR}/src/*.cpp)

# 创建可执行文件
add_executable(App ${SRC_LIST})
cmake_minimum_required(VERSION 3.10)

# 项目名称和版本
project(Vulkan_workspace VERSION 1.0)

# 指定 C++ 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 查找 Vulkan 和 GLFW 库
find_package(Vulkan REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

# 自动查找 src 文件夹中的所有 cpp 文件
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# 编译 glad.c
#add_library(glad src/glad.c)

# 添加可执行文件（main.cpp 包含在 SOURCES 中，自动作为入口点）
add_executable(Vulkan_workspace ${SOURCES})

# 链接 Vulkan、GLFW 和 GLM 库
target_link_libraries(Vulkan_workspace PRIVATE Vulkan::Vulkan glfw glm::glm glad::glad)

# 指定包含目录(include 目录中存放 .hpp 文件)
target_include_directories(Vulkan_workspace PRIVATE ${PROJECT_SOURCE_DIR}/include)
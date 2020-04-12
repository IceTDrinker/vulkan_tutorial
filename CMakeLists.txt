cmake_minimum_required(VERSION 3.10)

project(vulkan_tutorial VERSION 0.1)

set(EXECUTABLE_NAME "vulkan_tutorial")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Source files with Configure_depends, triggers a rebuild if new files are found
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "${SRC_DIR}/*.h")
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "${SRC_DIR}/*.cpp")
file(GLOB_RECURSE EDITOR_CFG_FILES CONFIGURE_DEPENDS "${SRC_DIR}/.editorconfig")

# For filters in VS 2019
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEADER_FILES} ${SRC_FILES} ${EDITOR_CFG_FILES})

# GLFW path
set(GLFW_ROOT "default value" CACHE PATH "Path to the glfw repo root")

# GLM path
# modification to have the glm directory in an include directory to avoid polluting the include search path
set(GLM_ROOT "default value" CACHE PATH "Path to the glm repo root")

# Vulkan
find_package(Vulkan REQUIRED)

# Executable
add_executable(${EXECUTABLE_NAME} ${HEADER_FILES} ${SRC_FILES} ${EDITOR_CFG_FILES})

# GLFW configuration
add_subdirectory("${GLFW_ROOT}" "thirdparty/glfw")
target_include_directories(${EXECUTABLE_NAME} SYSTEM PRIVATE "${GLFW_ROOT}/include")
target_link_libraries(${EXECUTABLE_NAME} glfw ${GLFW_LIBRARIES})

# GLM configuration
target_include_directories(${EXECUTABLE_NAME} SYSTEM PRIVATE "${GLM_ROOT}/include")

# Vulkan Configuration
target_compile_definitions(${EXECUTABLE_NAME} PRIVATE VK_USE_PLATFORM_WIN32_KHR)
target_include_directories(${EXECUTABLE_NAME} SYSTEM PRIVATE Vulkan::Vulkan)
target_link_libraries(${EXECUTABLE_NAME} Vulkan::Vulkan)


# Max warning level, warning = errors
target_compile_options(${EXECUTABLE_NAME} PRIVATE /W4 /WX)

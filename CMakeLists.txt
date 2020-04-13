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

# MSVC specifics
if(MSVC)
    # For filters in VS 2019
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEADER_FILES} ${SRC_FILES} ${EDITOR_CFG_FILES})
endif()

# GLFW path
set(GLFW_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw" CACHE PATH "Path to the glfw repo root")

# Vulkan
find_package(Vulkan REQUIRED)

# Executable
add_executable(${EXECUTABLE_NAME} ${HEADER_FILES} ${SRC_FILES} ${EDITOR_CFG_FILES})

# GLFW configuration
add_subdirectory("${GLFW_ROOT}" "thirdparty/glfw")
target_include_directories(${EXECUTABLE_NAME} SYSTEM PRIVATE "${GLFW_ROOT}/include")
target_link_libraries(${EXECUTABLE_NAME} glfw ${GLFW_LIBRARIES})

# Add thirdparty_include with symlink to GLM (this is to avoid glm polluting intelisense include helper in visual studio)
target_include_directories(${EXECUTABLE_NAME} SYSTEM PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty_include")

# Vulkan Configuration
target_compile_definitions(${EXECUTABLE_NAME} PRIVATE VK_USE_PLATFORM_WIN32_KHR)
target_include_directories(${EXECUTABLE_NAME} SYSTEM PRIVATE Vulkan::Vulkan)
target_link_libraries(${EXECUTABLE_NAME} Vulkan::Vulkan)

# MSVC specifics
if(MSVC)
    # Remove the console when running the program
    # set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
    # Max warning level without going insane, warning = errors
    target_compile_options(${EXECUTABLE_NAME} PRIVATE /W4 /WX)
endif()

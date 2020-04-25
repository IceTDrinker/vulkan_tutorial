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
file(GLOB_RECURSE SHDR_VERT_SRC_FILES CONFIGURE_DEPENDS "${SRC_DIR}/*.vert")
file(GLOB_RECURSE SHDR_FRAG_SRC_FILES CONFIGURE_DEPENDS "${SRC_DIR}/*.frag")

# MSVC specifics
if(MSVC)
    # For filters in VS 2019
    # All sources that are listed here must be added to a target to actually appear in Visual Studio
    source_group(
        TREE "${CMAKE_CURRENT_SOURCE_DIR}"
        FILES ${HEADER_FILES} ${SRC_FILES} ${EDITOR_CFG_FILES} ${SHDR_VERT_SRC_FILES} ${SHDR_FRAG_SRC_FILES}
    )
endif()

# Output variable for the function below (ugly I know but there are no return values in CMake and I did not find solutions online)
set(SHADER_COMPILED_FILES "")
# Automate glslc invokation
function(invoke_glslc SHADER_FILES SHADER_FILENAME_SUFFIX OUTPUT_DIR OUT_DIR_STAMP_FILE)
    foreach(SHADER_FILE IN ITEMS "${SHADER_FILES}")
        get_filename_component(SHADER_FILE_BASENAME "${SHADER_FILE}" NAME_WE)
        set(SHADER_FILE_SPIRV_OUTNAME "${OUTPUT_DIR}${SHADER_FILE_BASENAME}_${SHADER_FILENAME_SUFFIX}.spv")
        add_custom_command(
            OUTPUT "${SHADER_FILE_SPIRV_OUTNAME}"
            COMMAND glslc "${SHADER_FILE}" -o "${SHADER_FILE_SPIRV_OUTNAME}"
            DEPENDS "${OUT_DIR_STAMP_FILE}" "${SHADER_FILE}"
            VERBATIM
        )
        set(SHADER_COMPILED_FILES "${SHADER_COMPILED_FILES}" "${SHADER_FILE_SPIRV_OUTNAME}" PARENT_SCOPE)
    endforeach()
endfunction()

set(PRE_BUILD_TARGET prebuild)
set(SHADER_BC "${CMAKE_CURRENT_SOURCE_DIR}/shader_bc/")
set(SHADER_BC_STMP "${SHADER_BC}shader_bc.stamp")

invoke_glslc("${SHDR_VERT_SRC_FILES}" vert "${SHADER_BC}" "${SHADER_BC_STMP}")
invoke_glslc("${SHDR_FRAG_SRC_FILES}" frag "${SHADER_BC}" "${SHADER_BC_STMP}")

add_custom_command(
    OUTPUT "${SHADER_BC_STMP}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${SHADER_BC}"
    COMMAND ${CMAKE_COMMAND} -E touch "${SHADER_BC_STMP}"
    VERBATIM
)
add_custom_target(${PRE_BUILD_TARGET} DEPENDS "${SHADER_COMPILED_FILES}")

# GLFW path
set(GLFW_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glfw")

# Vulkan
find_package(Vulkan REQUIRED)

# Executable
add_executable(${EXECUTABLE_NAME} ${HEADER_FILES} ${SRC_FILES} ${EDITOR_CFG_FILES} ${SHDR_VERT_SRC_FILES} ${SHDR_FRAG_SRC_FILES})
add_dependencies(${EXECUTABLE_NAME} ${PRE_BUILD_TARGET})

# GLFW configuration
add_subdirectory("${GLFW_ROOT}")
target_include_directories(${EXECUTABLE_NAME} SYSTEM PRIVATE "${GLFW_ROOT}/include")
target_link_libraries(${EXECUTABLE_NAME} glfw ${GLFW_LIBRARIES})

# GLM include
target_include_directories(${EXECUTABLE_NAME} SYSTEM PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glm")

# Vulkan Configuration
# Trust CMake https://cmake.org/cmake/help/latest/module/FindVulkan.html rather than some old official doc
target_compile_definitions(${EXECUTABLE_NAME} PRIVATE VK_USE_PLATFORM_WIN32_KHR)
target_include_directories(${EXECUTABLE_NAME} SYSTEM PRIVATE ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${EXECUTABLE_NAME} ${Vulkan_LIBRARIES})

# MSVC specifics
if(MSVC)
    # Remove the console when running the program
    # set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

    # Max warning level, warning = errors
    # C4710 is linked to function not being inlined, why is this a warning ?
    # C4711 is linked to function being inlined, why is this a warning ?
    # C4820 is linked to padding added to struct which we don't care about here
    # C5045 is linked to spectre mitigation, we don't care about it here
    target_compile_options(${EXECUTABLE_NAME} PRIVATE /Wall /WX /wd4710 /wd4711 /wd4820 /wd5045)

    # MSVC has a problem with some min and max macros colliding with the functions from algorithm : http://www.suodenjoki.dk/us/archive/2010/min-max.htm
    add_compile_definitions(NOMINMAX)

    # Set working directory to easily access shader bytecode
    set_target_properties(${EXECUTABLE_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
endif()

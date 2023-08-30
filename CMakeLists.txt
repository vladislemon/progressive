cmake_minimum_required(VERSION 3.27)
project(progressive C)

set(CMAKE_C_STANDARD 11)

find_package(glfw3 3.3 REQUIRED)
find_package(cglm REQUIRED)

add_compile_definitions(
        GLFW_INCLUDE_NONE
        LODEPNG_NO_COMPILE_ENCODER
        LODEPNG_NO_COMPILE_ANCILLARY_CHUNKS
)

include_directories(include)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.c)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} glfw)
if (UNIX)
    target_link_libraries(${PROJECT_NAME} m)
endif (UNIX)
if (WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE $<CONFIG:Release>)
endif (WIN32)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND $<$<CONFIG:release>:${CMAKE_STRIP}> ARGS $<TARGET_FILE:${PROJECT_NAME}>)

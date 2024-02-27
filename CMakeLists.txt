cmake_minimum_required(VERSION 3.27.7)
project(SIMD-MANDELBROT)

set(IMGUI ./lib/imgui/)
set(IMGUI_BACKEND ${IMGUI}backends/)

add_subdirectory(lib/SDL EXCLUDE_FROM_ALL)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE /entry:mainCRTStartup")

file(GLOB_RECURSE src_files
    src/*.cpp
    ${IMGUI}imgui.cpp
    ${IMGUI}imgui_draw.cpp
    ${IMGUI}imgui_tables.cpp
    ${IMGUI}imgui_widgets.cpp
    ${IMGUI}imgui_demo.cpp
    ${IMGUI_BACKEND}imgui_impl_sdl3.cpp
    ${IMGUI_BACKEND}imgui_impl_sdlrenderer3.cpp
)

add_executable(${CMAKE_PROJECT_NAME} WIN32 ${src_files})

file(GLOB_RECURSE inc_files
    include/*/
    
)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    inc_files
    ${IMGUI}
    ${IMGUI_BACKEND}
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL3::SDL3)

ADD_CUSTOM_COMMAND(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}> $<TARGET_RUNTIME_DLLS:${CMAKE_PROJECT_NAME}>
    COMMAND_EXPAND_LISTS
)
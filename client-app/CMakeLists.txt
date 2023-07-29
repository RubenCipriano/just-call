cmake_minimum_required(VERSION 3.0)

project(just-call)

find_package(imgui CONFIG REQUIRED)

find_package(glfw3 CONFIG REQUIRED)

set(SOURCES src/main.cpp src/network.h src/utils.h src/models.h src/forms.h)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui glfw)
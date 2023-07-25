# CMakeList.txt: projeto de CMake para client-app, incluir origem e definir
# lógica específica a um projeto aqui.
#
find_package(imgui REQUIRED)
find_package(glfw3 REQUIRED)

# Adicione a origem ao executável deste projeto.
add_executable (client-app "client-app.cpp"  "utils.h" "network.h")

target_link_libraries(client-app PUBLIC imgui::imgui glfw)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET client-app PROPERTY CXX_STANDARD 20)
endif()

# TODO: Adicione testes e instale destinos, se necessário.

cmake_minimum_required(VERSION 3.20)
project(Lab4cube)

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_PREFIX_PATH "C:/GLEW" "C:/GLFW")
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)
include_directories("C:/GLFW/include" "C:/GLEW/include" "C:/msys64/mingw64/include/GL")
link_directories("C:/GLFW/lib-mingw-w64" "C:/GLEW/lib/Release/x64" "C:/msys64/mingw64/lib")

add_executable(Lab4cube main.cpp Visual.h Pochmann.h Cube.h GenAlgo.h imgui/imgui.cpp imgui/imgui_impl_glut.cpp
        imgui/imgui_demo.cpp imgui/imgui_draw.cpp imgui/imgui_impl_opengl2.cpp
        imgui/imgui_tables.cpp imgui/imgui_widgets.cpp imgui/imgui_impl_opengl3.cpp imfilebrowser.h)

target_link_libraries(Lab4cube ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} glfw3 glu32 opengl32 glew32)

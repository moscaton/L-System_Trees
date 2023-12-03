cmake_minimum_required(VERSION 3.8)

project(termProj)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_PREFIX_PATH
    /encs # For ENCS lab computers
    /opt/local # Macports
)

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)


set(SRC
    src/main.cpp    
    src/Branch.cpp   
    src/Leaf.cpp
)

set(ALL_LIBS
    GLEW::glew
    OpenGL::GL
    glfw
)

# NOTE: ENCS glm installation is missing links to *.inl files so we need this line
include_directories(/encs/pkg/glm-0.9.9.8/root/include)
include_directories(glm/glm/glm.hpp)
include_directories(glm/glm/gtc/matrix_transform.hpp)

add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME} ${ALL_LIBS})

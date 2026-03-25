cmake_minimum_required(VERSION 3.8)
project(Assignment2) # The name of your choice for the project comes here

add_compile_options(-DSTUDENT_SOLUTION)


IF(WIN32)
# set stuff for windows
find_package(OpenGL REQUIRED)


# for windows
include_directories(C:/vcpkg/packages/glfw3_x64-windows/include/)
include_directories(C:/vcpkg/packages/glew_x64-windows/include/)
include_directories(C:/vcpkg/packages/glew_x64-windows/include/GL/)
include_directories(C:/vcpkg/packages/glm_x64-windows/include/)

link_directories(C:/vcpkg/packages/glew_x64-windows/lib)
link_directories(C:/vcpkg/packages/glfw3_x64-windows/lib)



include_directories(src/)
aux_source_directory(src SOURCE)

#internal includes
aux_source_directory(external SOURCE)
aux_source_directory(src SOURCE)



add_executable(${PROJECT_NAME} main.cpp ${SOURCE} "src/Intersect.h" "src/Intersect.cpp") #The name of the cpp file and its path can vary

target_link_libraries(${PROJECT_NAME} ${OPENGL_gl_LIBRARY} glew32s glfw3)

ELSE()


set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_PREFIX_PATH
    /encs # For ENCS lab computers
    /opt/local # Macports
)

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED
    HINTS /encs/pkg/glfw-3.3.4/root # ENCS installation of glfw
)

# NOTE: ENCS glm installation is missing links to *.inl files so we need this line
include_directories(/encs/pkg/glm-0.9.9.8/root/include)

include_directories(src/)
aux_source_directory(src SOURCE)

#internal includes
aux_source_directory(external SOURCE)
aux_source_directory(src SOURCE)


add_executable(${PROJECT_NAME} main.cpp ${SOURCE}) #The name of the cpp file and its path can vary

target_link_libraries(${PROJECT_NAME} OpenGL::GL GLEW::glew glfw)

ENDIF()
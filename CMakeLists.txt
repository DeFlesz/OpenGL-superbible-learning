cmake_minimum_required(VERSION 3.0.0)

project(cmakeGL)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(extern/glfw)
add_subdirectory(extern/glm)

# hacky solution to get glad workin, as the original library has very interesting cmake solution
set(HEADER_FILES ${PROJECT_SOURCE_DIR}/extern/glad/include/glad/glad.h ${PROJECT_SOURCE_DIR}/extern/glad/include/KHR/khrplatform.h)

add_library(glad
	${PROJECT_SOURCE_DIR}/extern/glad/src/glad.c

	${HEADER_FILES}
)

target_include_directories(glad PRIVATE ${PROJECT_SOURCE_DIR}/extern/glad/include)

#src
add_executable(
    ${PROJECT_NAME}
    src/main.cpp
    src/Shader.cpp
    src/Renderer.cpp
    src/VertexBuffer.cpp
    src/VertexBufferLayout.cpp
    src/ElementBuffer.cpp
    src/VertexArrayObject.cpp
    src/Texture.cpp
    src/stb_image.cpp
)

target_include_directories(${PROJECT_NAME} 
    PUBLIC extern/glfw/include
    PUBLIC extern/glad/include
    PUBLIC include
)

target_link_directories(cmakeGL
    PRIVATE extern/glfw/src
    PRIVATE extern/glad/src
    PRIVATE src
)

target_link_libraries(${PROJECT_NAME} glfw glm glad)

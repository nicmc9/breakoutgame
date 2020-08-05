cmake_minimum_required(VERSION 3.10.0)
project(Breakout VERSION 0.1.0)

include(CTest)
enable_testing()



file(GLOB SOURCES "src/*.cpp")
add_executable(Breakout  ${SOURCES}	external/GLAD/src/glad.c )

add_subdirectory(${CMAKE_SOURCE_DIR}/external/GLM)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/GLFW)

#find_package(Freetype REQUIRED)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/freetype)
target_include_directories(Breakout PRIVATE ${FREETYPE_INCLUDE_DIRS})

include_directories(${CMAKE_SOURCE_DIR}/external/GLM)
include_directories(${CMAKE_SOURCE_DIR}/external/GLAD/include)
include_directories(${CMAKE_SOURCE_DIR}/external/GLFW/include)
include_directories(${CMAKE_SOURCE_DIR}/external/STB)
include_directories(${CMAKE_SOURCE_DIR}/external/freetype/include)
#include_directories(${CMAKE_SOURCE_DIR}/external/irrKlang/include)
 


target_link_libraries(Breakout  glfw ${GLFW_LIBRARIES} )
target_link_libraries(Breakout freetype ${FREETYPE_LIBRARIES})




set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# Based on TrinityCore CMake files
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

project(ModernGameEngine)

file(GLOB_RECURSE sources_Pool Pool/*.cpp     ${CMAKE_SOURCE_DIR}/include/Pool/*.hpp)
file(GLOB_RECURSE sources_Scheduler Scheduler/*.cpp ${CMAKE_SOURCE_DIR}/include/Scheduler/*.hpp)
file(GLOB_RECURSE sources_Shader Shader/*.cpp ${CMAKE_SOURCE_DIR}/include/Shader/*.hpp)
file(GLOB_RECURSE sources_Utils Utils/*.cpp   ${CMAKE_SOURCE_DIR}/include/Utils/*.hpp)
file(GLOB_RECURSE sources_Window Window/*.cpp ${CMAKE_SOURCE_DIR}/include/Window/*.hpp)

set(EPS_SRCS
  ${EPS_SRCS}
  ${sources_Pool}
  ${sources_Scheduler}
  ${sources_Shader}
  ${sources_Utils}
  ${sources_Window}
)

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/include/Pool
  ${CMAKE_SOURCE_DIR}/include/Scheduler
  ${CMAKE_SOURCE_DIR}/include/Shader
  ${CMAKE_SOURCE_DIR}/include/Utils
  ${CMAKE_SOURCE_DIR}/include/Window

  ${CMAKE_SOURCE_DIR}/dep/glfw/include
  ${CMAKE_SOURCE_DIR}/dep/glew/include
  ${CMAKE_SOURCE_DIR}/dep/glm
)

add_definitions(-DGLEW_STATIC)

add_library(ModernGameEngine
  ${EPS_SRCS}
)

set(ModernGameEngine_LIBRARIES
  ${THREAD_LIB}
  glfw
  glew_s
  ${GLFW_LIBRARIES}
  ModernGameEngine
)

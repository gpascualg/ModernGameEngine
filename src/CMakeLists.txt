# Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

project(lfs)

file(GLOB_RECURSE sources_Game Game/*.cpp Game/*.h)
file(GLOB_RECURSE sources_Pool Pool/*.cpp Pool/*.h)
file(GLOB_RECURSE sources_Server Server/*.cpp Server/*.h)
file(GLOB_RECURSE sources_Utils Utils/*.cpp Utils/*.h)
file(GLOB_RECURSE sources_Window Window/*.cpp Window/*.h)

set(EPS_SRCS
  ${EPS_SRCS}
  ${sources_Game}
  ${sources_Pool}
  ${sources_Server}
  ${sources_Utils}
  ${sources_Window}
)

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/Game
  ${CMAKE_CURRENT_SOURCE_DIR}/Pool
  ${CMAKE_CURRENT_SOURCE_DIR}/Server
  ${CMAKE_CURRENT_SOURCE_DIR}/Utils
  ${CMAKE_CURRENT_SOURCE_DIR}/Window

  ${CMAKE_SOURCE_DIR}/dep/glfw/include
)

add_executable(lfs
  ${EPS_SRCS}
)

target_link_libraries(lfs
  ${THREAD_LIB}
  glfw
  ${GLFW_LIBRARIES}
)

install()

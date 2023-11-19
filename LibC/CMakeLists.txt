add_library(LibC "")

target_sources(LibC
  PRIVATE 
  ${CMAKE_CURRENT_LIST_DIR}/stdio/printf.cpp
  ${CMAKE_CURRENT_LIST_DIR}/stdio/putchar.cpp

  PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/include/stdio.h
  ${CMAKE_CURRENT_LIST_DIR}/include/test.h
)

target_include_directories(LibC
    PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}
  )



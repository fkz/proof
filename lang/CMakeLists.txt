
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

ADD_CUSTOM_COMMAND(
   OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/parse.cc
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/Parser.h" "${CMAKE_CURRENT_BINARY_DIR}/Parser.h"
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/Parser.ih" "${CMAKE_CURRENT_BINARY_DIR}/Parser.ih"
   COMMAND bisonc++ -V -H "kern/element_ptr.h" ${CMAKE_CURRENT_SOURCE_DIR}/grammar.y
   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/grammar.y) # ${CMAKE_CURRENT_SOURCE_DIR}/expression.h)

ADD_EXECUTABLE(parser Parser.cpp parse.cc lex.cpp main.cpp)
target_link_libraries(parser kern)
project(my_proof)

add_definitions(-Wall)

include_directories(.)



add_subdirectory(lang)
add_subdirectory(kern)

add_executable(my_proof main.cpp)
target_link_libraries(my_proof kern)
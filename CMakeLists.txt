project(my_proof)

add_definitions(-Wall -O0 -g)

include_directories(.)

set(CMAKE_BUILD_TYPE Release)


add_subdirectory(lang)
add_subdirectory(kern)

add_executable(my_proof main.cpp)
target_link_libraries(my_proof kern)
add_subdirectory(samples)
project(testlib)
project(test)

add_executable(test main.cpp)
add_library(testlib SHARED test.cpp)

set_target_properties(testlib PROPERTIES OUTPUT_NAME test)
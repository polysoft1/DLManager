cmake_minimum_required(VERSION 3.0.2)

project(testlib)
project(test)

add_executable(test main.cpp)
add_library(testlib SHARED test.cpp)

set_target_properties(testlib PROPERTIES OUTPUT_NAME test)
set_target_properties(testlib PROPERTIES PREFIX "")

set_property(TARGET test PROPERTY CXX_STANDARD 17)

if(UNIX)
	# Link dynamic shared lib loading lib.
	target_link_libraries(test ${CMAKE_DL_LIBS})
endif()
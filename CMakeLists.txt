cmake_minimum_required (VERSION 2.8.11)
project (consumer_data_privacy_gca)
set(NAME "consumer_data_privacy_gca")

find_path(CMAKE_FOLDER NAMES cmake/emp-tool-config.cmake)

include(${CMAKE_FOLDER}/cmake/common.cmake)
include(${CMAKE_FOLDER}/cmake/source_of_randomness.cmake)
include(${CMAKE_FOLDER}/cmake/threading.cmake)

FIND_PACKAGE(emp-ot REQUIRED)
INCLUDE_DIRECTORIES(${EMP-OT_INCLUDE_DIRS})

install(DIRECTORY consumer_data_privacy_gca DESTINATION include)
install(DIRECTORY cmake/ DESTINATION cmake)

# Test cases
macro (add_test _name)
	add_test_with_lib(${_name} ${EMP-OT_LIBRARIES})
endmacro()

add_test(circuittool)
add_test(final)
add_test(Frame_Match)
add_test(Frame_Match_V2)

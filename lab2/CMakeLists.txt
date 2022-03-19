cmake_minimum_required(VERSION 3.20)
project(Lab2_1)

list(APPEND CMAKE_PREFIX_PATH "libcurl/lib")

find_package(CURL REQUIRED)
include_directories("C:/Users/asd/Desktop/Lab2.1/libcurl/include/curl", "C:/Users/asd/Desktop/Lab2.1/nlohmann")

set(CMAKE_CXX_STANDARD 17)

add_executable(Lab2_1 main.cpp VariadicTable.h parse.h currency.h pretty_table.h)

target_link_libraries(Lab2_1 CURL::libcurl)





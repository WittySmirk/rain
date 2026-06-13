cmake_minimum_required(VERSION 3.10)

project(rain CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(SQLite3 REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")

add_executable(rain ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(${PROJECT_NAME} PRIVATE
	SQLite3::SQLite3
)

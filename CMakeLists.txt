cmake_minimum_required(VERSION 3.15)
project(PneumoniaWuhan2020)

set(CMAKE_CXX_STANDARD 17)

if (CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-Wall)
    message(STATUS "Add compile flags: -Wall")
    add_compile_options(-Wextra)
    message(STATUS "Add compile flags: -Wextra")
endif()


include_directories(include)
add_executable(PneumoniaWuhan2020 src/main.cpp src/Statistics.cpp src/AreaStat.cpp)
target_link_libraries(PneumoniaWuhan2020 -lcpr -lsqlite3 -lpthread)
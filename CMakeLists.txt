cmake_minimum_required(VERSION 3.10)

project(midterm-m10815822)

set(OpenCV_DIR "C:\\opencv\\build")
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(midterm-m10815822
    src/midterm-m10815822.cpp
    src/estimatefootprint.cpp
)

target_link_libraries(midterm-m10815822 ${OpenCV_LIBS})

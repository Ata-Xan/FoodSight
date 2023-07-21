# cmake_minimum_required(VERSION 3.10)
# project(FoodSight VERSION 0.1.0)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS OFF)

# include(CTest)
# include_directories( ${OpenCV_INCLUDE_DIRS} )
# find_package( OpenCV 4 REQUIRED )
# enable_testing()

# add_executable(FoodSight src/main.cpp)
# target_link_libraries( FoodSight ${OpenCV_LIBS} )

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)

cmake_minimum_required(VERSION 3.0)
project(FoodSight VERSION 0.1.0)
# Set OpenCV directory if installed in non-standard location
set(OpenCV_DIR "/usr/local/lib/cmake/opencv4")
find_package(OpenCV REQUIRED COMPONENTS core highgui features2d xfeatures2d)
# find_package(OpenCV REQUIRED)
# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
# Find OpenCV
find_package(OpenCV REQUIRED)

# Add include directories
include_directories(${OpenCV_INCLUDE_DIRS})

# Create executable
add_executable(FoodSight src/main.cpp)

# Link libraries
# target_link_libraries(FoodSight ${OpenCV_LIBS})

target_link_libraries(FoodSight ${OpenCV_LIBS} opencv_xfeatures2d stdc++fs)

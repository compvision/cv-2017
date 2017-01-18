find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED COMPONENTS system thread)

add_executable(cv-net cv_b.cpp NetworkController.cpp)
target_link_libraries(cv-net ${OpenCV_LIBS} ${Boost_LIBRARIES})

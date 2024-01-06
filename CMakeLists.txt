cmake_minimum_required(VERSION 3.8)
project(yolox_trt)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(CUDA REQUIRED)
find_package(OpenCV REQUIRED)
link_directories(~/TensorRT-8.6.1.6/targets/x86_64-linux-gnu/lib) # change to your TensorRT path
set(TensorRT_INCLUDE_DIR ~/TensorRT-8.6.1.6/targets/x86_64-linux-gnu/include) # change to your TensorRT path

include_directories(
  include
  ${CUDA_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
  ${TensorRT_INCLUDE_DIR}
)

add_library(${PROJECT_NAME} SHARED src/yolox_tensorrt.cpp)
target_link_libraries(${PROJECT_NAME} nvinfer nvinfer_plugin nvparsers nvonnxparser ${OpenCV_LIBS} ${CUDA_LIBRARIES})

add_executable(yolox_trt_example example/yolox_trt_example.cpp)
target_link_libraries(yolox_trt_example ${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

install(
  DIRECTORY include/
  DESTINATION include
)

install(
  TARGETS ${PROJECT_NAME}
  DESTINATION lib
)

install(
  TARGETS yolox_trt_example
  DESTINATION lib/${PROJECT_NAME}
)

ament_export_include_directories(include)
ament_export_libraries(${PROJECT_NAME})
ament_package()

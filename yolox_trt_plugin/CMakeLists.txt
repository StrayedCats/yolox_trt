cmake_minimum_required(VERSION 3.8)
project(yolox_trt_plugin)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake_auto REQUIRED)
ament_auto_find_build_dependencies()
find_package(CUDA REQUIRED)
find_package(OpenCV REQUIRED)

link_directories(~/TensorRT-8.6.1.6/targets/x86_64-linux-gnu/lib) # change to your TensorRT path
set(TensorRT_INCLUDE_DIR ~/TensorRT-8.6.1.6/targets/x86_64-linux-gnu/include) # change to your TensorRT path

file(GLOB_RECURSE SRCS src/*.cpp)
ament_auto_add_library(yolox_trt_plugin SHARED
  ${SRCS}
)
target_include_directories(yolox_trt_plugin PUBLIC
  ${CUDA_INCLUDE_DIRS}
  ${TensorRT_INCLUDE_DIR}
)
ament_target_dependencies(${PROJECT_NAME} OpenCV)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

pluginlib_export_plugin_description_file(detector2d_base yolox_trt_plugin.xml)

include_directories(include)
ament_auto_package()

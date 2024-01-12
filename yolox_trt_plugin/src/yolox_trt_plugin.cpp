// Copyright 2023 StrayedCats.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "yolox_trt_plugin/yolox_trt_plugin.hpp"

namespace detector2d_plugins
{

void YoloxTrt::init(const detector2d_parameters::ParamListener & param_listener)
{
  (void)param_listener;
  
  //TODO : get path to engine from param_listener
  std::string path_to_engine_ = "./src/yolox_trt/weight/yolox_tiny.trt";
  yolo = std::make_shared<yolox_trt::YoloXTensorRT>(path_to_engine_);
  
  //TODO : get bool from param_listener
}

Detection2DArray YoloxTrt::detect(const cv::Mat & image)
{
  (void)image;

  std::cout << "YoloxTrt::detect" << std::endl;

  auto objects = yolo->inference(image);
  yolox_trt::utils::draw_objects(image, objects);

  cv::imshow("yolox", image);
  auto key = cv::waitKey(1);
  if (key == 27)
  {
    rclcpp::shutdown();
  }

  Detection2DArray pose;
  return pose;
  
  //TODO : get bool from param_listener
}
}

#include <pluginlib/class_list_macros.hpp>
PLUGINLIB_EXPORT_CLASS(detector2d_plugins::YoloxTrt, detector2d_base::Detector)
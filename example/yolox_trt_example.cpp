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

#include "yolox_cpp/yolox_tensorrt.hpp"
#include "yolox_cpp/utils.hpp"

int main(int argc, char** argv)
{
    if(argc < 3){
        std::cerr << "Usage: " << argv[0] << " <path_to_engine> <path_to_image>" << std::endl;
        return -1;
    }

    yolox_cpp::YoloXTensorRT yolo(argv[1]);
    std::string image_path = argv[2];
    std::string image_path_out = image_path.substr(0, image_path.find_last_of(".")) + "_out.jpg";

    cv::Mat frame = cv::imread(image_path);
    auto objects = yolo.inference(frame);

    yolox_cpp::utils::draw_objects(frame, objects);
    cv::imwrite(image_path_out, frame);

    return 0;
}
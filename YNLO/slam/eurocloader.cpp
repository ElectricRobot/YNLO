#include "eurocloader.h"
#include <fstream>
#include "yaml.h"

namespace ynlo {

EurocLoader::EurocLoader() = default;

EurocLoader::EurocLoader(const std::string& file_path_) {
    Init(file_path_);
}

EurocLoader::~EurocLoader() = default;

void EurocLoader::Init(const std::string& file_path_) {
    left_image_filename.clear();
    right_image_filename.clear();
    pose.clear();
    timestamps.clear();

    std::string cam0_file_path = file_path_ + "cam0/";
    std::string cam0_sensor_filename = cam0_file_path + "sensor.yaml";
    YamlParser t(cam0_sensor_filename);

    std::string cam1_file_path = file_path_ + "cam1/";
    YamlParser t2(cam1_file_path+"sensor.yaml");
}

}

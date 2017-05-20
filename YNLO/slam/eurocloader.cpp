#include "eurocloader.h"
#include <fstream>

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
    std::ifstream ifs(cam0_sensor_filename);
    if(!ifs.is_open()) {
        std::cout << "Cannot open the file: " << cam0_sensor_filename << std::endl;
        return;
    }
    std::string line;
    while(std::getline(ifs, line)) {

    }
    std::string cam1_file_path = file_path_ + "cam1/";
}

}

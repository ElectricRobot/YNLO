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

    std::string sensor_type;
    double rate_hz;
    cv::Mat resolution;
    cv::Mat T_BS;

    t["sensor_type"] >> sensor_type;
    t["rate_hz"] >> rate_hz;
    t["resolution"] >> resolution;
    t["T_BS"] >> T_BS;

    std::cout << sensor_type <<std::endl;
    std::cout << rate_hz << std::endl;
    std::cout << resolution << std::endl;
    std::cout << T_BS << std::endl;
    //std::string cam1_file_path = file_path_ + "cam1/";
    //YamlParser t2(cam1_file_path+"sensor.yaml");
}

}

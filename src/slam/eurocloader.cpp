#include "eurocloader.h"
#include <fstream>
#include <sstream>
#include "yaml.h"

namespace ynlo {

EurocLoader::EurocLoader() = default;

EurocLoader::EurocLoader(const std::string& file_path_) {
    Init(file_path_);
}

EurocLoader::~EurocLoader() = default;

void EurocLoader::Init(const std::string& file_path_) {
    left_image_filename_.clear();
    right_image_filename_.clear();
    pose_.clear();
    timestamps_.clear();

    cv::Mat resolution, intrinsics;

    std::string cam0_file_path = file_path_ + "cam0/";
    std::string cam0_sensor_filename = cam0_file_path + "sensor.yaml";
    YamlParser yaml(cam0_sensor_filename);
    yaml["T_BS"] >> Tbc_[0];

    yaml["resolution"] >> resolution;
    image_size_.width = resolution.at<float>(0);
    image_size_.height = resolution.at<float>(1);

    yaml["intrinsics"] >> intrinsics;
    K_[0] = cv::Mat::eye(3,3,CV_32F);
    K_[0].at<float>(0,0) = intrinsics.at<float>(0);
    K_[0].at<float>(1,1) = intrinsics.at<float>(1);
    K_[0].at<float>(0,2) = intrinsics.at<float>(2);
    K_[0].at<float>(1,2) = intrinsics.at<float>(3);

    yaml["distortion_coefficients"] >> D_[0];

    std::string cam1_file_path = file_path_ + "cam1/";
    std::string cam1_sensor_filename = cam1_file_path + "sensor.yaml";
    yaml.Open(cam1_sensor_filename);
    yaml["T_BS"] >> Tbc_[1];
    yaml["intrinsics"] >> intrinsics;
    K_[1] = cv::Mat::eye(3,3,CV_32F);
    K_[1].at<float>(0,0) = intrinsics.at<float>(0);
    K_[1].at<float>(1,1) = intrinsics.at<float>(1);
    K_[1].at<float>(0,2) = intrinsics.at<float>(2);
    K_[1].at<float>(1,2) = intrinsics.at<float>(3);

    yaml["distortion_coefficients"] >> D_[1];

    T21_ = Tbc_[1].inv()*Tbc_[0];

    std::string time_stamps_filename = cam0_file_path + "data.csv";
    std::ifstream ifs(time_stamps_filename);
    {
        std::string line;
        char image_filename[65535];
        double timestamp;
        std::getline(ifs, line);
        while(std::getline(ifs, line)) {
            sscanf(line.c_str(), "%lf,%s", &timestamp, image_filename);
            timestamps_.push_back(timestamp);
            left_image_filename_.push_back(cam0_file_path+"data/"+image_filename);
            right_image_filename_.push_back(cam1_file_path+"data/"+image_filename);
        }
    }

    N_ = timestamps_.size();
}

const cv::Mat& EurocLoader::Kl() const {
    return K_[0];
}

const cv::Mat& EurocLoader::Kr() const {
    return K_[1];
}

const cv::Mat& EurocLoader::Tbcl() const {
    return Tbc_[0];
}

const cv::Mat& EurocLoader::Tbc2() const {
    return Tbc_[1];
}

const cv::Mat& EurocLoader::Trl() const {
    return T21_;
}

const cv::Size& EurocLoader::ImageSize() const {
    return image_size_;
}

const std::vector<double>& EurocLoader::TimeStamps() const {
    return timestamps_;
}

int EurocLoader::SequenceSize() const {
    return N_;
}

cv::Mat EurocLoader::LeftImage(int i) const {
    if(i < 0 || i >= N_)
        return cv::Mat();
    return cv::imread(left_image_filename_[i], cv::IMREAD_UNCHANGED);
}

cv::Mat EurocLoader::RightImage(int i) const {
    if(i < 0 || i >= N_)
        return cv::Mat();
    return cv::imread(right_image_filename_[i], cv::IMREAD_UNCHANGED);
}

const cv::Mat& EurocLoader::Dl() const {
    return D_[0];
}

const cv::Mat& EurocLoader::Dr() const {
    return D_[1];
}

}

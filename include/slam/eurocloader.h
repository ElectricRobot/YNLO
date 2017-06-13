#ifndef EUROCLOADER_H
#define EUROCLOADER_H
#include <string>
#include <array>
#include <vector>
#include <opencv2/opencv.hpp>

namespace ynlo {

class EurocLoader
{
public:
    EurocLoader();
    EurocLoader(const std::string& file_path_);
    ~EurocLoader();

    void Init(const std::string& file_path_);

private:
    std::array<cv::Mat, 2> K_, D_;
    cv::Mat T21_;
    std::array<cv::Mat, 2> Tbc_;
    cv::Size image_size_;
    std::string file_path_;
    std::vector<double> timestamps_;
    std::vector<std::string> left_image_filename_;
    std::vector<std::string> right_image_filename_;
    std::vector<cv::Mat> pose_;
};

}
#endif // EUROCLOADER_H

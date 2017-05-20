#ifndef EUROCLOADER_H
#define EUROCLOADER_H
#include <string>
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

    cv::Mat K0, K1;
    cv::Mat D0, D1;
    cv::Mat T21;
    cv::Size image_size;
    std::string file_path;
    std::vector<double> timestamps;
    std::vector<std::string> left_image_filename;
    std::vector<std::string> right_image_filename;
    std::vector<cv::Mat> pose;
private:

};

}
#endif // EUROCLOADER_H

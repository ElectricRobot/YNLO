#ifndef KITTILOADER_H
#define KITTILOADER_H
#include <string>
#include <fstream>
#include <vector>
#include <opencv2/opencv.hpp>

namespace ynlo {

class KITTILoader
{
public:
    KITTILoader(const std::string& _basedir, const std::string& _sequence);

    std::string basedir;
    std::string sequence;

    cv::Mat P0, P1;
    std::vector<double> timestamps;
    std::vector<std::string> leftImageFilename;
    std::vector<std::string> rightImageFilename;
    std::vector<cv::Mat> poses;
    int sequenceSize;

private:
    bool loadCamera(const std::string& filename);
    void loadProjectionMatrix(std::ifstream& fin, cv::Mat& P);

    bool loadTimes(const std::string& filename);
    void loadImageFilename();

    bool loadPose(const std::string& filename);
    std::string num2string(const int num);
};

}
#endif // KITTILOADER_H

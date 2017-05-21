#ifndef UTIL_H
#define UTIL_H
#include <string>
#include <opencv2/opencv.hpp>

namespace ynlo {
std::string EraseDummySpace(const std::string& line);
std::string EraseTheStringAfterSpecificChar(const std::string& line, char token);
cv::Mat String2CvMat(const std::string& string_mat);
}

#endif // UTIL_H

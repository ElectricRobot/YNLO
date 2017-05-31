#ifndef UTIL_H
#define UTIL_H
#include <string>
#include <memory>
#include <opencv2/opencv.hpp>

#define SMART_PTR(T) using SPtr = std::shared_ptr<T>;\
    using WPtr = std::weak_ptr<T>;\
    using UPtr = std::unique_ptr<T>;

namespace ynlo {
std::string EraseDummySpace(const std::string& line);
std::string EraseTheStringAfterSpecificChar(const std::string& line, char token);
cv::Mat String2CvMat(const std::string& string_mat);
cv::Mat String2CvMat(const std::string& string_mat, int rows, int cols);
}

#endif // UTIL_H

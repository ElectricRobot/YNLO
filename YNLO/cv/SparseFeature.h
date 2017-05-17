#ifndef SPARSEFEATURE_H
#define SPARSEFEATURE_H
#include <vector>
#include <opencv2/opencv.hpp>

namespace ynlo {

class SparseFeature {
public:
    std::vector<cv::KeyPoint> kps_, un_kps_;
    cv::Mat desc_;
};

}

#endif // SPARSEFEATURE_H

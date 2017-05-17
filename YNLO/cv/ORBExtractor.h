#ifndef ORBEXTRACTOR_H
#define ORBEXTRACTOR_H
#include <opencv2/opencv.hpp>
#include "CamParam.h"
#include "SparseFeature.h"

namespace ynlo {

class ORBExtractor
{
public:
    ORBExtractor(int num_features = 500, float scale_factor = 1.2f, int num_levels = 8,
                 int edge_threshold = 31, int first_level = 0, int WTA_K = 2,
                 int score_type = cv::ORB::HARRIS_SCORE, int patch_size = 32, int fast_threshold = 20)
        : num_features_(num_features), scale_factor_(scale_factor), num_levels_(num_levels),
          edge_threshold_(edge_threshold), first_level_(first_level), WTA_K_(WTA_K),
          score_type_(score_type), patch_size_(patch_size), fast_threshold_(fast_threshold)
    {
        orb_ = cv::ORB::create(num_features_, scale_factor_, num_levels_, edge_threshold_,
                               first_level_, WTA_K_, score_type_, patch_size_, fast_threshold_);
    }

    ~ORBExtractor() {}

    void DetectAndCompute(cv::Mat image, cv::Mat mask, SparseFeature& features) {
        orb_->detectAndCompute(image, mask, features.kps_, features.desc_);
    }

    void DetectAndCompute(cv::Mat image, cv::Mat mask, const CamParam& cam , SparseFeature& features) {
        orb_->detectAndCompute(image, mask, features.kps_, features.desc_);

        if(cam.is_fish_eye_) {
            cv::fisheye::undistortPoints(features.kps_, features.un_kps_, cam.K_, cam.D_, cv::noArray(), cam.K_);
        }
        else {
            cv::undistortPoints(features.kps_, features.un_kps_, cam.K_, cam.D_, cv::noArray(), cam.K_);
        }
    }

private:
    cv::Ptr<cv::ORB> orb_;
    int num_features_; // 500
    float scale_factor_; // 1.2f
    int num_levels_; // 8
    int edge_threshold_; // 31
    int first_level_; // 0
    int WTA_K_; // 2
    int score_type_; // ORB::HARRIS_SCORE
    int patch_size_; // 32
    int fast_threshold_; // 20
};

}
#endif // ORBEXTRACTOR_H

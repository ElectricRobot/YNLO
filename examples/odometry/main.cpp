#include <iostream>
#include <string>
#include <sstream>
#include <opencv2/opencv.hpp>
#include "slam/kittiloader.h"
#include "slam/eurocloader.h"
#include "cvutil.h"
using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
    //ynlo::KITTILoader loader("/home/liu/dataset/Odometry/KITTI/dataset/", "00");
    ynlo::EurocLoader loader("/home/liu/dataset/Odometry/euroc/V2_01_easy/");

    cv::Mat Kl = loader.Kl(), Dl = loader.Dl();
    cv::Mat Kr = loader.Kr(), Dr = loader.Dr();
    cv::Mat Trl = loader.Trl();
    cv::Size image_size = loader.ImageSize();

    std::cout << image_size << std::endl;
    std::cout << Kl << std::endl;
    std::cout << Dl << std::endl;
    std::cout << Kr << std::endl;
    std::cout << Dr << std::endl;
    std::cout << Trl << std::endl;
    cv::Mat Rl, Rr, Pl, Pr;
    cv::stereoRectify(Kl, Dl, Kr, Dr, image_size, Trl.colRange(0,3).rowRange(0,3), Trl.col(3).rowRange(0,3),
            Rl, Rr, Pl, Pr, cv::noArray(), CALIB_ZERO_DISPARITY, 1, image_size);

    std::cout << Rl << endl << Rr << endl << Pl << endl << Pr << endl;

    for(int i = 0, n = loader.SequenceSize(); i < n; ++i) {
        cv::Mat left_image = loader.LeftImage(i);
        cv::Mat right_image = loader.RightImage(i);
        std::vector<cv::KeyPoint> kps_l, kps_r;
        std::vector<cv::Mat> img_pyd_l, img_pyd_r;
        ynlo::BasicImageProcessing(left_image, 5, 20, 64, kps_l, img_pyd_l);
        ynlo::BasicImageProcessing(right_image, 5, 20, 64, kps_r, img_pyd_r);

        std::vector<cv::Point2f> un_kps_l, un_kps_r;
        auto matches = ynlo::SparseStereoMatching(img_pyd_l, kps_l, img_pyd_r, kps_r,
                Kl, Dl, Rl, Pl, Kr, Dr, Rr, Pr, un_kps_l, un_kps_r);

        std::vector<cv::DMatch> draw_match;
        for(auto& it : matches) {
            draw_match.push_back(DMatch(it.first, it.second, 0));
        }

        cv::Mat res;
        cv::drawMatches(left_image, kps_l, right_image, kps_r, draw_match, res);
        cv::imshow("res", res);
        cv::waitKey(0);
    }

    return 0;
}

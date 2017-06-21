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

    cout << loader.Kl() << endl;
    cout << loader.Kr() << endl;
    cout << loader.Dl() << endl;
    cout << loader.Dr() << endl;
    cout << loader.Trl() << endl;

    for(int i = 0, n = loader.SequenceSize(); i < n; ++i) {
        cv::Mat left_image = loader.LeftImage(i);

        std::vector<cv::KeyPoint> kps;
        std::vector<cv::Mat> img_pyd;
        ynlo::BasicImageProcessing(left_image, 5, 20, 64, kps, img_pyd);

    }

    return 0;
}

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
        cv::Mat right_image = loader.RightImage(i);
        auto img_pyd = ynlo::Pyramid(left_image, 5);

        for(int i = 0, n = img_pyd.size(); i < n; ++i) {
            std::stringstream ss;
            ss << i;
            cv::imshow(ss.str(), img_pyd[i]);
        }
        cv::waitKey(0);
    }

    return 0;
}

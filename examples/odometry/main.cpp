#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include "slam/kittiloader.h"
#include "slam/eurocloader.h"
using namespace std;
using namespace cv;

int main(int argc, char **argv)
{

    //ynlo::KITTILoader loader("/home/liu/dataset/Odometry/KITTI/dataset/", "00");
    ynlo::EurocLoader loader("/home/liu/dataset/Odometry/euroc/V2_01_easy/");

    return 0;
}

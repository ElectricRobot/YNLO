/*
 * cvutil.cpp
 *
 *  Created on: Jun 15, 2017
 *      Author: liu
 */
#include "cvutil.h"

namespace ynlo {

std::vector<cv::Mat> Pyramid(cv::Mat& img, int levels) {
    if(levels < 1)
        return std::vector<cv::Mat>();

    std::vector<cv::Mat> img_pyd;
    img_pyd.resize(levels);
    img_pyd[0] = img;

    for(int i = 1; i < levels; ++i) {
        cv::pyrDown(img_pyd[i-1], img_pyd[i]);
    }

    return img_pyd;
}

}

/*
 * cvutil.h
 *
 *  Created on: Jun 15, 2017
 *      Author: liu
 */

#ifndef CVUTIL_H
#define CVUTIL_H
#include <opencv2/opencv.hpp>

namespace ynlo {

std::vector<cv::Mat> Pyramid(cv::Mat& img, int levels);

}

#endif /* CVUTIL_H */

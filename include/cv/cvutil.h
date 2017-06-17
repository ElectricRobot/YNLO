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

float ShiTomasiScore(const cv::Mat& img, int u, int v);

std::vector<cv::KeyPoint> StrongFastCorner(const cv::Mat& img, int fast_th, int col_grid_window_size, int row_grid_window_size);

void BasicImageProcessing(cv::Mat& img, int levels, int fast_th, int grid_size, std::vector<cv::KeyPoint>& kps, std::vector<cv::Mat>& img_pyd);

}

#endif /* CVUTIL_H */

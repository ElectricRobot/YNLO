/*
 * cvutil.h
 *
 *  Created on: Jun 15, 2017
 *      Author: liu
 */

#ifndef CVUTIL_H
#define CVUTIL_H
#include <vector>
#include <climits>
#include <opencv2/opencv.hpp>

namespace ynlo {

std::vector<cv::Mat> Pyramid(cv::Mat& img, int levels);

float ShiTomasiScore(const cv::Mat& img, int u, int v);

std::vector<cv::KeyPoint> StrongFastCorner(const cv::Mat& img, int fast_th, int col_grid_window_size, int row_grid_window_size);

void BasicImageProcessing(cv::Mat& img, int levels, int fast_th, int grid_window_size, std::vector<cv::KeyPoint>& kps, std::vector<cv::Mat>& img_pyd);

std::vector<std::pair<int, int>> SparseStereoMatching(
        const std::vector<cv::Mat>& img_pyd_l, std::vector<cv::KeyPoint>& kps_l,
        const std::vector<cv::Mat>& img_pyd_r, std::vector<cv::KeyPoint>& kps_r,
        const cv::Mat& Kl, const cv::Mat& Dl, const cv::Mat& Rl, const cv::Mat& Pl,
        const cv::Mat& Kr, const cv::Mat& Dr, const cv::Mat& Rr, const cv::Mat& Pr
        );

template<class T>
double interpolate(const cv::Mat& src, double u, double v) {
    assert(src.type() == CV_8U || src.type() == CV_32F || src.type() == CV_64F);
    int ui = std::floor(u);
    int vi = std::floor(v);

    if(ui < 0 || ui >= src.cols || vi < 0 || vi >= src.rows)
        return 0.0f;

    double k1 = u - ui;
    double k2 = v - vi;

    bool f1 = ui < src.cols - 1;
    bool f2 = vi < src.rows - 1;

    double value = (1.0f-k1)*(1.0f-k2)*static_cast<double>(src.at<T>(vi, ui)) +
            (f1 ? (k1*(1.0f-k2)*static_cast<double>(src.at<T>(vi, ui+1))):0.0f) +
            (f2 ? ((1.0f-k1)*k2*static_cast<double>(src.at<T>(vi+1, ui))):0.0f) +
            ((f1 && f2) ? (k1*k2*static_cast<double>(src.at<T>(vi+1, ui+1))):0.0f);
    return value;
}

}

#endif /* CVUTIL_H */

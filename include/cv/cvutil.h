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
        const std::vector<cv::Mat>& img_pyd_l, const std::vector<cv::KeyPoint>& kps_l,
        const std::vector<cv::Mat>& img_pyd_r, const std::vector<cv::KeyPoint>& kps_r,
        const cv::Mat& Kl, const cv::Mat& Dl, const cv::Mat& Rl, const cv::Mat& Pl,
        const cv::Mat& Kr, const cv::Mat& Dr, const cv::Mat& Rr, const cv::Mat& Pr,
        std::vector<cv::Point2f>& un_kps_l, std::vector<cv::Point2f>& un_kps_r)
{
    std::vector<std::pair<int, int>> matches;
    cv::KeyPoint::convert(kps_l, un_kps_l);
    cv::KeyPoint::convert(kps_r, un_kps_r);
    cv::undistortPoints(un_kps_l, un_kps_l, Kl, Dl, Rl, Pl);
    cv::undistortPoints(un_kps_r, un_kps_r, Kr, Dr, Rr, Pr);

    // epipolar line constrain
    cv::Size image_size = img_pyd_l[0].size();
    std::vector<std::vector<size_t>> table(image_size.height);

    for(int i = 0, n = un_kps_r.size(); i < n; ++i) {
        cv::Point2f& pt = un_kps_r[i];
        int level = kps_r[i].octave;
        int scale = 1 << level;
        double r = 2.0f*scale;
        int minr = std::floor(pt.y - r);
        int maxr = std::ceil(pt.y + r);

        for(int y = minr; y <= maxr; ++y) {
            if(y < 0 || y >= image_size.height)
                continue;
            table[y].push_back(i);
        }
    }

    // matching
    for(int iL = 0, nL = kps_l.size(); iL < nL; ++iL) {
        const cv::KeyPoint& kpl = kps_l[iL];
        const cv::Point2f& un_kpl = un_kps_l[iL];
        int level = kpl.octave;
        int scale = (1 << level);
        double inv_scale = 1.0f/scale;
        const cv::Mat& imgl = img_pyd_l[level];
        cv::Point2f scale_kpl = kpl.pt*inv_scale;
        int w = 5;

        if(scale_kpl.x - w < 0 || scale_kpl.y - w < 0 || scale_kpl.x + w >= imgl.cols || scale_kpl.y + w >= imgl.rows)
            continue;

        cv::Mat IL(imgl.colRange(scale_kpl.x - w, scale_kpl.x + w + 1).rowRange(scale_kpl.y - w, scale_kpl.y + w + 1));
        IL.convertTo(IL, CV_64F);
        IL = IL - cv::Mat::eye(IL.size(), CV_64F)*IL.at<double>(w, w);

        if(un_kpl.y < 0 || un_kpl.y >= image_size.height)
            continue;

        const std::vector<size_t>& candidate = table[un_kpl.y];

        double best_score = 3000, second_score = best_score;
        int best_iR = -1;
        for(int iC = 0, nC = candidate.size(); iC < nC; ++iC) {
            size_t iR = candidate[iC];
            const cv::KeyPoint& kpr = kps_r[iR];

            if(kpr.octave != level)
                continue;

            cv::Mat imgr = img_pyd_r[level];
            cv::Point2f scale_kpr = kpr.pt * inv_scale;

            if(scale_kpr.x - w < 0 || scale_kpr.y - w < 0 || scale_kpr.x + w >= imgr.cols || scale_kpr.y + w >= imgr.rows)
                continue;

            cv::Mat IR(imgl.colRange(scale_kpl.x - w, scale_kpl.x + w + 1).rowRange(scale_kpl.y - w, scale_kpl.y + w + 1));
            IR.convertTo(IR, CV_64F);
            IR = IR - cv::Mat::eye(IR.size(), CV_64F)*IR.at<double>(w, w);
            double score = cv::norm(IL, IR, cv::NORM_L1);

            if(score < best_score) {
                second_score = best_score;
                best_score = score;
                best_iR = iR;
            }
            else if(score < second_score) {
                second_score = score;
            }
        }

        if(best_iR == -1)
            continue;

        double ratio = 0.6f;
        if(best_score > second_score*ratio)
            continue;

        const cv::Point2f& un_kpr = un_kps_r[best_iR];
        double disparity = un_kpl.x - un_kpr.x;
        const double& fx = Pl.at<const double>(0,0);

        if(disparity < 0.0f || disparity >= fx) {
            continue;
        }

        matches.push_back(std::make_pair(iL, best_iR));
    }

    // TODO
    // patch alignment (subpixel refinement)
    /*
    int levels = img_pyd_l.size();
    std::vector<std::vector<cv::Point2f>> group_points(levels);
    for(auto& it : matches) {
        int level = kps_l[it.first].octave;

    }
    */
    return matches;
}

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

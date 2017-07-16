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

float ShiTomasiScore(const cv::Mat& img, int u, int v) {
    assert(img.type() == CV_8UC1);

    float dXX = 0.0;
    float dYY = 0.0;
    float dXY = 0.0;
    const int halfbox_size = 4;
    const int box_size = 2*halfbox_size;
    const int box_area = box_size*box_size;
    const int x_min = u-halfbox_size;
    const int x_max = u+halfbox_size;
    const int y_min = v-halfbox_size;
    const int y_max = v+halfbox_size;

    if(x_min < 1 || x_max >= img.cols-1 || y_min < 1 || y_max >= img.rows-1)
      return 0.0; // patch is too close to the boundary

    const int stride = img.step.p[0];
    for( int y=y_min; y<y_max; ++y )
    {
      const uint8_t* ptr_left   = img.data + stride*y + x_min - 1;
      const uint8_t* ptr_right  = img.data + stride*y + x_min + 1;
      const uint8_t* ptr_top    = img.data + stride*(y-1) + x_min;
      const uint8_t* ptr_bottom = img.data + stride*(y+1) + x_min;
      for(int x = 0; x < box_size; ++x, ++ptr_left, ++ptr_right, ++ptr_top, ++ptr_bottom)
      {
        float dx = *ptr_right - *ptr_left;
        float dy = *ptr_bottom - *ptr_top;
        dXX += dx*dx;
        dYY += dy*dy;
        dXY += dx*dy;
      }
    }

    // Find and return smaller eigenvalue:
    dXX = dXX / (2.0 * box_area);
    dYY = dYY / (2.0 * box_area);
    dXY = dXY / (2.0 * box_area);
    return 0.5 * (dXX + dYY - sqrt( (dXX + dYY) * (dXX + dYY) - 4 * (dXX * dYY - dXY * dXY) ));
}

std::vector<cv::KeyPoint> StrongFastCorner(const cv::Mat& img, int fast_th, int col_grid_window_size, int row_grid_window_size) {
    assert(img.type() == CV_8UC1);

    std::vector<cv::KeyPoint> kps;
    cv::FAST(img, kps, fast_th);

    int col_grid_size = (img.cols % col_grid_window_size == 0)? img.cols/col_grid_window_size : img.cols/col_grid_window_size + 1;
    int row_grid_size = (img.rows % row_grid_window_size == 0)? img.rows/row_grid_window_size : img.rows/row_grid_window_size + 1;

    using Index = int;
    using Score = float;
    using Grids = std::vector<std::vector<std::pair<Index, Score>>>;
    Grids grids;

    grids.resize(row_grid_size);
    for(auto& it : grids) {
        it.resize(col_grid_size, std::make_pair(-1, 0.0f));
    }

    for(int i = 0, n = kps.size(); i < n; ++i) {
        auto& kp = kps[i];
        int col_index = kp.pt.x / col_grid_window_size;
        int row_index = kp.pt.y / row_grid_window_size;
        auto& best_corner = grids[row_index][col_index];
        auto score = ShiTomasiScore(img, kp.pt.x, kp.pt.y);

        if(score > best_corner.second) {
            best_corner.second = score;
            best_corner.first = i;
        }
    }

    std::vector<cv::KeyPoint> strong_kps;
    for(auto& ity : grids) {
        for(auto& it : ity) {
            if(it.first != -1)
                strong_kps.push_back(kps[it.first]);
        }
    }

    return strong_kps;
}

void BasicImageProcessing(cv::Mat& img, int levels, int fast_th, int grid_window_size, std::vector<cv::KeyPoint>& kps, std::vector<cv::Mat>& img_pyd) {

    img_pyd = Pyramid(img, levels);

    std::vector<std::vector<cv::KeyPoint>> kps_by_levels;
    kps_by_levels.resize(levels);

    for(int i = 0; i < levels; ++i) {
        kps_by_levels[i] = StrongFastCorner(img_pyd[i], fast_th, grid_window_size, grid_window_size);
    }

    kps.clear();

#if 0
    std::vector<cv::Mat> debug_draw;
    debug_draw.resize(levels);
#endif

    for(int i = 0; i < levels; ++i) {
        float scale = (1 << i);
        for(int j = 0, m = kps_by_levels[i].size(); j < m; ++j) {
            auto kp = kps_by_levels[i][j];
            kp.pt *= scale;
            kp.octave = i;
            kps.push_back(kp);
        }
#if 0
        cv::drawKeypoints(img_pyd[i], kps_by_levels[i], debug_draw[i], cv::Scalar(0,255,0));
        std::stringstream ss;
        ss << i;
        cv::imshow(ss.str(), debug_draw[i]);
#endif
    }
#if 0
    cv::waitKey(0);
#endif
}

std::vector<std::pair<int, int>> SparseStereoMatching(
        const std::vector<cv::Mat>& img_pyd_l, std::vector<cv::KeyPoint>& kps_l,
        const std::vector<cv::Mat>& img_pyd_r, std::vector<cv::KeyPoint>& kps_r,
        const cv::Mat& Kl, const cv::Mat& Dl, const cv::Mat& Rl, const cv::Mat& Pl,
        const cv::Mat& Kr, const cv::Mat& Dr, const cv::Mat& Rr, const cv::Mat& Pr
        )
{
    std::vector<cv::Point2f> un_kps_l;
    std::vector<cv::Point2f> un_kps_r;
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

    // patch alignment (subpixel refinement)
    std::vector<cv::Point2f> align_pts(kps_r.size());
    std::vector<std::pair<int, int>> align_matches;
    for(auto& it : matches) {
        int level = kps_l[it.first].octave;
        double scale = (1 << level);
        double inv_scale = 1.0f/scale;
        cv::Mat img1 = img_pyd_l[level];
        cv::Mat img2 = img_pyd_r[level];
        std::vector<cv::Point2f> kp1{kps_l[it.first].pt};
        kp1[0] *= inv_scale;
        std::vector<cv::Point2f> kp2{kps_r[it.second].pt};
        kp2[0] *= inv_scale;

        std::vector<uchar> status;
        std::vector<float> err;

        cv::calcOpticalFlowPyrLK(img1, img2, kp1, kp2, status, err, cv::Size(11,11),
                0, cv::TermCriteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 30, 1e-5),
                cv::OPTFLOW_USE_INITIAL_FLOW);

        if(status[0] == 0 || err[0] > 20)
            continue;

        kp2[0] *= scale;
        align_pts[it.second] = kp2[0];
        align_matches.push_back(std::make_pair(it.first, it.second));
    }

#if 0
    cv::Mat drawl, drawr;
    cv::cvtColor(img_pyd_l[0], drawl, CV_GRAY2BGR);
    cv::cvtColor(img_pyd_r[0], drawr, CV_GRAY2BGR);

    for(auto& it : align_matches) {
        cv::circle(drawl, kps_l[it.first].pt, 2, cv::Scalar(0,255,0),-1);
        cv::circle(drawr, kps_r[it.second].pt, 2, cv::Scalar(0,255,0),-1);
        cv::line(drawr, kps_r[it.second].pt, align_pts[it.second], cv::Scalar(255,0,0),2);
        cv::circle(drawr, align_pts[it.second], 2, cv::Scalar(0,0,255),-1);
    }

    cv::imshow("drawl", drawl);
    cv::imshow("drawr", drawr);
    cv::waitKey(0);
#endif

    for(auto& it : align_matches) {
        kps_r[it.second].pt = align_pts[it.second];
    }
    return align_matches;
}


}

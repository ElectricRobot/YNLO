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
    assert(img.type() == CV_8U);

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

void BasicImageProcessing(cv::Mat& img, int levels, int fast_th, int grid_size, std::vector<cv::KeyPoint>& kps, std::vector<cv::Mat>& img_pyd) {

}


}

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include "Camera.h"
using namespace std;
using namespace cv;

int main(int argc, char** argv) {

    if(argc != 3) {
        cerr  << endl << "[usage]\n./" << argv[0] << " camera.yaml img.png" << endl;
        return 1;
    }

    cv::FileStorage fs(argv[1], cv::FileStorage::READ);
    float fx, fy, cx, cy;
    float k1, k2, p1, p2;
    fx = fs["Camera.fx"];
    fy = fs["Camera.fy"];
    cx = fs["Camera.cx"];
    cy = fs["Camera.cy"];

    k1 = fs["Camera.k1"];
    k2 = fs["Camera.k2"];
    p1 = fs["Camera.p1"];
    p2 = fs["Camera.p2"];
    fs.release();

    cv::Mat K = cv::Mat::eye(3,3,CV_32F);
    K.at<float>(0,0) = fx;
    K.at<float>(1,1) = fy;
    K.at<float>(0,2) = cx;
    K.at<float>(1,2) = cy;
    cv::Mat D = (cv::Mat_<float>(4,1)<<k1,k2,p1,p2);

    cv::Mat img = cv::imread(argv[2]);

    cv::Size image_size;
    image_size.height = img.rows;
    image_size.width = img.cols;

    ynlo::Camera::SPtr cam = make_shared<ynlo::PinholeCamera>();
    cam->SetImageSize(image_size);
    cam->SetCameraModel(K, D);
    cv::Mat undistort_img;
    cam->UndistortImage(img, undistort_img);

    cv::Ptr<cv::ORB> orb = cv::ORB::create(1200);
    std::vector<cv::KeyPoint> kps, un_kps;
    orb->detect(img, kps);
    std::vector<cv::Point2f> ps, un_ps;

    for(int i = 0, n = kps.size(); i < n; ++i) {
        ps.push_back(kps[i].pt);
    }
    cam->UndistortPoints(ps, un_ps);
    std::cout << ps.size() << " " << un_ps.size() << std::endl;

    for(int i = 0, n = kps.size(); i < n; ++i) {
        if(un_ps[i].x < 0 || un_ps[i].x > image_size.width) {
            continue;
        }
        un_kps.push_back(cv::KeyPoint(un_ps[i], kps[i].size, kps[i].angle, kps[i].response, kps[i].octave, kps[i].class_id));
    }
    cout << kps.size() << " " << un_kps.size() << endl;

    cv::drawKeypoints(img, kps, img);
    cv::drawKeypoints(undistort_img, un_kps, undistort_img);

    cv::imshow("1", img);
    cv::imshow("2", undistort_img);
    cv::waitKey(0);
    return 0;
}

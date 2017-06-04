/*
 * Camera.cpp
 *
 *  Created on: Jun 2, 2017
 *      Author: liu
 */
#include "Camera.h"

namespace ynlo {

Camera::Camera() {
}

Camera::Camera(const Camera& rhs) :
        image_size_(rhs.image_size_), K_(rhs.K_.clone()), D_(rhs.D_.clone()),
        undistort_map1_(rhs.undistort_map1_.clone()),
        undistort_map2_(rhs.undistort_map2_.clone())
{
}

Camera::Camera(Camera&& rhs) :
        image_size_(std::move(rhs.image_size_)), K_(std::move(rhs.K_)), D_(std::move(rhs.D_)),
        undistort_map1_(std::move(rhs.undistort_map1_)),
        undistort_map2_(std::move(rhs.undistort_map2_))
{
}

Camera::~Camera() {
}

Camera& Camera::operator=(const Camera& rhs) {
    image_size_ = rhs.image_size_;
    K_ = rhs.K_.clone();
    D_ = rhs.D_.clone();
    undistort_map1_ = rhs.undistort_map1_.clone();
    undistort_map2_ = rhs.undistort_map2_.clone();
    return *this;
}

Camera& Camera::operator=(Camera&& rhs) {
    image_size_ = std::move(rhs.image_size_);
    K_ = std::move(rhs.K_);
    D_ = std::move(rhs.D_);
    undistort_map1_ = std::move(rhs.undistort_map1_);
    undistort_map2_ = std::move(rhs.undistort_map2_);
    return *this;
}

const cv::Size& Camera::ImageSize() const {
    return image_size_;
}

const cv::Mat& Camera::K() const {
    return K_;
}

const cv::Mat& Camera::D() const {
    return D_;
}

void Camera::SetImageSize(const cv::Size& image_size) {
    image_size_ = image_size;
}

void Camera::SetCameraModel(const cv::Mat& K, const cv::Mat& D) {
    if(image_size_.height == 0 || image_size_.width == 0)
        std::cerr << "Please set the image_size before setting camera model." << std::endl;
    K_ = K.clone();
    D_ = D.clone();
    InitUndistortMap();
}

void Camera::UndistortImage(cv::InputArray distorted, cv::OutputArray undistorted, int interpolation) {
    cv::remap(distorted, undistorted, undistort_map1_, undistort_map2_, interpolation);
}

PinholeCamera::PinholeCamera() : Camera::Camera() {};
PinholeCamera::PinholeCamera(const PinholeCamera& rhs)
    : Camera::Camera(rhs)
{}

PinholeCamera::PinholeCamera(PinholeCamera&& rhs)
    : Camera::Camera(std::move(rhs))
{}

PinholeCamera::~PinholeCamera()
{}

PinholeCamera& PinholeCamera::operator=(const PinholeCamera& rhs) {
    Camera::operator=(rhs);
    return *this;
}

PinholeCamera& PinholeCamera::operator=(PinholeCamera&& rhs) {
    Camera::operator =(std::move(rhs));
    return *this;
}

void PinholeCamera::UndistortPoints(cv::InputArray distorted, cv::OutputArray undistorted) {
    cv::undistortPoints(distorted, undistorted, K_, D_, cv::noArray(), K_);
}

void PinholeCamera::InitUndistortMap() {
    cv::initUndistortRectifyMap(K_, D_, cv::noArray(), K_, image_size_, CV_32F, undistort_map1_, undistort_map2_);
}

FisheyeCamera::FisheyeCamera(): Camera::Camera() {}
FisheyeCamera::FisheyeCamera(const FisheyeCamera& rhs)
    : Camera::Camera(rhs)
{}

FisheyeCamera::FisheyeCamera(FisheyeCamera&& rhs)
    : Camera::Camera(std::move(rhs))
{}

FisheyeCamera::~FisheyeCamera()
{}

FisheyeCamera& FisheyeCamera::operator=(const FisheyeCamera& rhs) {
    Camera::operator =(rhs);
    return *this;
}

FisheyeCamera& FisheyeCamera::operator=(FisheyeCamera&& rhs) {
    Camera::operator =(std::move(rhs));
    return *this;
}

void FisheyeCamera::UndistortPoints(cv::InputArray distorted, cv::OutputArray undistorted) {
    cv::fisheye::undistortPoints(distorted, undistorted, K_, D_, cv::noArray(), K_);
}

void FisheyeCamera::InitUndistortMap() {
    cv::fisheye::initUndistortRectifyMap(K_, D_, cv::noArray(), K_, image_size_, CV_32F, undistort_map1_, undistort_map2_);
}

}

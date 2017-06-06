/*
 * Camera.cpp
 *
 *  Created on: Jun 2, 2017
 *      Author: liu
 */
#include "Camera.h"
#include <cassert>
namespace ynlo {

Camera::Camera() {
}

Camera::Camera(const cv::Size& image_size, const cv::Mat& K, const cv::Mat& D) {
    assert(image_size.width > 0 || image_size.height > 0);
    assert(!K.empty());
    image_size_ = image_size;
    K_ = K.clone();
    D_ = D.clone();
}

Camera::Camera(const Camera& rhs) :
        image_size_(rhs.image_size_), K_(rhs.K_.clone()), D_(rhs.D_.clone()),
        undistort_map1_(rhs.undistort_map1_.clone()),
        undistort_map2_(rhs.undistort_map2_.clone()),
        is_init_(rhs.is_init_)
{
}

Camera::Camera(Camera&& rhs) :
        image_size_(std::move(rhs.image_size_)), K_(std::move(rhs.K_)), D_(std::move(rhs.D_)),
        undistort_map1_(std::move(rhs.undistort_map1_)),
        undistort_map2_(std::move(rhs.undistort_map2_)),
        is_init_(std::move(rhs.is_init_))
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
    is_init_ = rhs.is_init_;
    return *this;
}

Camera& Camera::operator=(Camera&& rhs) {
    image_size_ = std::move(rhs.image_size_);
    K_ = std::move(rhs.K_);
    D_ = std::move(rhs.D_);
    undistort_map1_ = std::move(rhs.undistort_map1_);
    undistort_map2_ = std::move(rhs.undistort_map2_);
    is_init_ = std::move(rhs.is_init_);
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
    if(!K_.empty())
        InitUndistortMap();
}

void Camera::SetCameraModel(const cv::Mat& K, const cv::Mat& D) {
    K_ = K.clone();
    D_ = D.clone();
    if(image_size_.height != 0 || image_size_.width != 0)
        InitUndistortMap();
}

void Camera::UndistortImage(cv::InputArray distorted, cv::OutputArray undistorted, int interpolation) {
    if(is_init_)
        cv::remap(distorted, undistorted, undistort_map1_, undistort_map2_, interpolation);
}

PinholeCamera::PinholeCamera() : Camera::Camera() {};
PinholeCamera::PinholeCamera(const cv::Size& image_size, const cv::Mat& K, const cv::Mat& D) : Camera::Camera(image_size, K, D)
{
    InitUndistortMap();
}

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
    is_init_ = true;
}

FisheyeCamera::FisheyeCamera(): Camera::Camera() {}
FisheyeCamera::FisheyeCamera(const cv::Size& image_size, const cv::Mat& K, const cv::Mat& D) : Camera::Camera(image_size, K, D)
{
    InitUndistortMap();
}
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
    is_init_ = true;
}

}

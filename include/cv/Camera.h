/*
 * Camera.h
 *
 *  Created on: Jun 1, 2017
 *      Author: liu
 */
#ifndef CAMERA_H
#define CAMERA_H
#include <memory>
#include <opencv2/opencv.hpp>
#include "util.h"

namespace ynlo {

class Camera {
public:
    SMART_PTR(Camera)

    /* Constructor */
    Camera();
    Camera(const cv::Size& image_size, const cv::Mat& K, const cv::Mat& D);
    Camera(const Camera&);
    Camera(Camera&&);
    virtual ~Camera();

    /* Assignment */
    Camera& operator=(const Camera&);
    Camera& operator=(Camera&&);

    /* getter */
    const cv::Size& ImageSize() const;
    const cv::Mat& K() const;
    const cv::Mat& D() const;
    /* setter */
    void SetImageSize(const cv::Size& image_size);
    void SetCameraModel(const cv::Mat& K, const cv::Mat& D = cv::Mat());

    /* Method */
    void UndistortImage(cv::InputArray distorted, cv::OutputArray undistorted, int interpolation = cv::INTER_LINEAR);
    virtual void UndistortPoints(cv::InputArray distorted, cv::OutputArray undistorted) = 0;

protected:
    virtual void InitUndistortMap() = 0;

    cv::Size image_size_;
    cv::Mat K_, D_;
    cv::Mat undistort_map1_, undistort_map2_;
    bool is_init_ = false;
};

class PinholeCamera : public Camera
{
public:
    SMART_PTR(PinholeCamera)

    /* Constructor */
    PinholeCamera();
    PinholeCamera(const cv::Size& image_size, const cv::Mat& K, const cv::Mat& D);
    PinholeCamera(const PinholeCamera&);
    PinholeCamera(PinholeCamera&&);
    ~PinholeCamera();

    /* Assignment */
    PinholeCamera& operator=(const PinholeCamera&);
    PinholeCamera& operator=(PinholeCamera&&);

    /* Method */
    void UndistortPoints(cv::InputArray distorted, cv::OutputArray undistorted) override;
private:
    void InitUndistortMap() override;
};

class FisheyeCamera : public Camera {
public:
    SMART_PTR(FisheyeCamera)

    /* Consturctor */
    FisheyeCamera();
    FisheyeCamera(const cv::Size& image_size, const cv::Mat& K, const cv::Mat& D);
    FisheyeCamera(const FisheyeCamera&);
    FisheyeCamera(FisheyeCamera&&);
    ~FisheyeCamera();

    /* Assignment */
    FisheyeCamera& operator=(const FisheyeCamera&);
    FisheyeCamera& operator=(FisheyeCamera&&);

    /* Method */
    void UndistortPoints(cv::InputArray distorted, cv::OutputArray undistorted) override;
private:
    void InitUndistortMap() override;
};
}

#endif /* CAMERA_H */

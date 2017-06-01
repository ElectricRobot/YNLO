/*
 * Frame.h
 *
 *  Created on: May 30, 2017
 *      Author: liu
 */

#ifndef FRAME_H
#define FRAME_H
#include <memory>
#include <opencv2/opencv.hpp>
#include <Eigen/Core>
#include <sophus/se3.hpp>
#include "Camera.h"
#include "Feature.h"
#include "util.h"

namespace ynlo {

class AbstractCamera;
class Feature;

/* A frame saves the image, the associated features and the estimated pose. */
class Frame {
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    SMART_PTR(Frame)

    /* Constructor */
    Frame();
    Frame(const Frame&);
    virtual ~Frame();

    /* Assignment */
    Frame& operator=(const Frame&);

    static int frame_counter_;              //!< Counts the number of created frames. Used to set the unique id.
    int id_;                                //!< Unique id of the frame.
    double timestamp_;                      //!< Timestamp of when the image was recorded.
    //AbstractCamera::SPtr cam_;              //!< Camera model.
    Sophus::SE3d Tfw_;                      //!< Transform (f)rame from (w)orld.
    Eigen::Matrix<double,6,6> cov_;         //!< Covariance.
    std::vector<cv::Mat> img_pyr_;          //!< Image Pyramid.
    std::vector<Feature::SPtr> fts_;        //!< List of features in the image.
    std::vector<Feature::WPtr> key_pts_;    //!< Five features and associated 3D points which are used to detect if two frames have overlapping field of view.
    bool is_keyframe_;                      //!< Was this frames selected as keyframe?
};

} /* namespace ynlo */

#endif /* FRAME_H */

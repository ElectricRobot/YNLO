/*
 * Feature.h
 *
 *  Created on: May 31, 2017
 *      Author: liu
 */

#ifndef FEATURE_H
#define FEATURE_H
#include <Eigen/Core>
#include "Frame.h"
#include "Point.h"

namespace ynlo {

class Frame;
class Point;

/// A salient image region that is tracked across frames.
class Feature {
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    using SPtr = std::shared_ptr<Feature>;
    using WPtr = std::weak_ptr<Feature>;
    using UPtr = std::unique_ptr<Feature>;

    enum FeatureType {
        CORNER,
    };

    /* Constructor */
    Feature();
    Feature(const Feature&);
    virtual ~Feature();

    /* Assignment */
    Feature& operator=(const Feature&);

    FeatureType type_;
    Frame::WPtr frame_;
    Eigen::Vector2d px_;
    int level_;
    Point::WPtr point_;
};

} /* namespace ynlo */

#endif /* FEATURE_H */

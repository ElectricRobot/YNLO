/*
 * Point.h
 *
 *  Created on: May 31, 2017
 *      Author: liu
 */

#ifndef POINT_H
#define POINT_H
#include <vector>
#include <Eigen/Core>
#include "Feature.h"
#include "util.h"

namespace ynlo {

class Feature;

/// A 3D point on the surface of the scene.
class Point {
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    SMART_PTR(Point)

    enum PointType {
        TYPE_DELETED,
        TYPE_CANDIDATE,
        TYPE_UNKNOWN,
        TYPE_GOOD
    };

    /* Constructor */
    Point();
    Point(const Point&);
    virtual ~Point();

    /* Assignment */
    Point& operator=(const Point&);

    static int point_counter_;
    int id_;
    Eigen::Vector3d pos_;
    Eigen::Vector3d normal_;
    Eigen::Matrix3d normal_information_;
    std::vector<Feature::WPtr> obs_;
    PointType type_;
};

} /* namespace ynlo */

#endif /* POINT_H */

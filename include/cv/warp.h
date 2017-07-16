/*
 * warp.h
 *
 *  Created on: Jul 10, 2017
 *      Author: liu
 */

#ifndef WARP_H
#define WARP_H
#include <opencv2/opencv.hpp>

class WarpBase {
public:
    WarpBase() {

    }

    virtual ~WarpBase() {

    }

    void SetWarpMatrix(const cv::Mat& W) {
        W_ = W.clone();
    }


protected:
    cv::Mat W_;
};

class WarpShift : public WarpBase
{
public:
    WarpShift() {

    }

    ~WarpShift() {

    }
private:

};

class WarpEuclid : public WarpBase
{
public:
    WarpEuclid() {

    }

    ~WarpEuclid() {

    }
private:

};

class WarpSimilar : public WarpBase
{
public:
    WarpSimilar() {

    }

    ~WarpSimilar() {

    }
private:

};

class WarpAffine : public WarpBase
{
public:
    WarpAffine() {

    }

    ~WarpAffine() {

    }
private:

};

class WarpProj : public WarpBase
{
public:
    WarpProj() {

    }

    ~WarpProj() {

    }
private:

};

#endif /* WARP_H */

#ifndef KEYFRAME_H
#define KEYFRAME_H
#include <map>
#include <memory>
#include "frame.h"

namespace ynlo {

class KeyFrame : virtual public Frame {
public:
    /* neighbor keyframe */
    std::map<std::weak_ptr<KeyFrame>, int> neighbor_keyframes_;
};

}

#endif // KEYFRAME_H

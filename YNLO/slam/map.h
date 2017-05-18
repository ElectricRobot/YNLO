#ifndef MAP_H
#define MAP_H
#include <unordered_set>
#include <memory>
#include "frame.h"
#include "keyframe.h"
#include "mappoint.h"

namespace ynlo {

class Map {
public:
    std::unordered_set<std::shared_ptr<KeyFrame>> keyframes_;
    std::unordered_set<std::shared_ptr<MapPoint>> mappoints_;
    std::shared_ptr<Frame> last_frame_;
};

}

#endif // MAP_H

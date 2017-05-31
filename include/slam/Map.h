/*
 * Map.h
 *
 *  Created on: May 31, 2017
 *      Author: liu
 */

#ifndef MAP_H
#define MAP_H
#include <vector>
namespace ynlo {

class Frame;
class Point;

// Map object which saves all keyframes which are in a map.
class Map {
public:
    /* Constructor */
    Map();
    Map(const Map&);
    virtual ~Map();

    /* Assignment */
    Map& operator=(const Map&);

    std::vector<Frame::SPtr> keyframes_;
    std::vector<Point::SPtr> mappoints_;
};

} /* namespace ynlo */

#endif /* MAP_H */

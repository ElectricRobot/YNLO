#ifndef TRACKING_H
#define TRACKING_H

namespace ynlo {

class Tracking {
public:
    void ExtractSparseFeature();
    void TrackWithLastFrame();
    void TrackWithLocalMap();
    void CheckInsertKeyFrame();
};

}

#endif // TRACKING_H

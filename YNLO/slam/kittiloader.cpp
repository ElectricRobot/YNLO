#include <iostream>
#include <sstream>
#include "kittiloader.h"
namespace ynlo {
using namespace std;
using namespace cv;

KITTILoader::KITTILoader(const std::string& _basedir, const std::string& _sequence)
{
    basedir = _basedir;
    sequence = _sequence;
    sequenceSize = 0;
    std::string calibFilename = basedir+"sequences/"+sequence+"/calib.txt";
    if(!loadCamera(calibFilename))
        return;

    std::string timesFilename = basedir+"sequences/"+sequence+"/times.txt";
    if(!loadTimes(timesFilename))
        return;

    loadImageFilename();

    std::string poseFilename = basedir+"poses/"+sequence+".txt";
    if(!loadPose(poseFilename))
        return;
}

bool KITTILoader::loadCamera(const std::string& filename) {
    ifstream finCalib(filename);
    if(!finCalib) {
        cout << "Cannot open file: " << filename << endl;
        return false;
    }
    loadProjectionMatrix(finCalib, P0);
    loadProjectionMatrix(finCalib, P1);
    finCalib.close();
    return true;
}

void KITTILoader::loadProjectionMatrix(std::ifstream& fin, cv::Mat& P) {
    string line, token;
    stringstream ss;
    getline(fin, line);
    P.create(3,4,CV_32F);
    ss.str(line);
    ss >> token >> P.at<float>(0,0) >> P.at<float>(0,1) >> P.at<float>(0,2) >> P.at<float>(0,3)
                >> P.at<float>(1,0) >> P.at<float>(1,1) >> P.at<float>(1,2) >> P.at<float>(1,3)
                >> P.at<float>(2,0) >> P.at<float>(2,1) >> P.at<float>(2,2) >> P.at<float>(2,3);
}

bool KITTILoader::loadTimes(const std::string& filename) {
    ifstream finTimes(filename);
    if(!finTimes) {
        cout << "Cannot open file: " << filename << endl;
        return false;
    }

    string line;
    while(getline(finTimes, line)) {
        timestamps.push_back(stod(line));
        ++sequenceSize;
    }
    finTimes.close();
    return true;
}

void KITTILoader::loadImageFilename() {
    assert(sequenceSize > 0);
    string leftImageFolder = basedir+"sequences/"+sequence+"/image_0/";
    string rightImageFolder = basedir+"sequences/"+sequence+"/image_1/";

    for(int i = 0; i < sequenceSize; ++i) {
        string filename = num2string(i) + ".png";
        leftImageFilename.push_back(leftImageFolder+filename);
        rightImageFilename.push_back(rightImageFolder+filename);
    }
}

bool KITTILoader::loadPose(const std::string& filename) {
    ifstream finPose(filename);
    if(!finPose) {
        cout << "Cannot open file: " << filename << endl;
        return false;
    }

    string line;
    while(getline(finPose, line)) {
        stringstream ss(line);
        poses.push_back(cv::Mat::eye(4,4,CV_32F));
        float* posePtr = poses.back().ptr<float>();
        for(int i = 0; i < 12; ++i) {
            ss >> posePtr[i];
        }
    }
    return true;
}

std::string KITTILoader::num2string(const int num) {
    stringstream ss;
    ss << num;
    string stringNum = ss.str();
    ss.str("");
    ss.clear();

    int numLeft0 = 6 - stringNum.size();

    for(int i = 0; i < numLeft0; ++i)
        ss << "0";

    ss << num;
    return ss.str();
}
}

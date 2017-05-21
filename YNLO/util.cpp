#include "util.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
namespace ynlo {

std::string EraseDummySpace(const std::string& line) {
    if(line.empty())
        return line;

    int begin_index = line.size()-1;

    for(int i = 0, n = line.size(); i < n; ++i) {
        if(isspace(line[i]) == 0) {
            begin_index = i;
            break;
        }
    }

    if(begin_index == line.size()-1) {
        return std::string();
    }

    int end_index = begin_index+1;

    for(int i = line.size(); i > begin_index; --i) {
        if(isspace(line[i-1]) == 0) {
            end_index = i;
            break;
        }
    }

    return std::string(line.begin()+begin_index, line.begin()+end_index);
}

std::string EraseTheStringAfterSpecificChar(const std::string& line, char token) {
    std::string deal_line(line);
    auto it = std::find(deal_line.begin(), deal_line.end(), token);
    if(it != deal_line.end())
        deal_line.erase(it, deal_line.end());
    return deal_line;
}

cv::Mat String2CvMat(const std::string& string_mat) {
    std::string deal_string_mat = string_mat;
    deal_string_mat = EraseDummySpace(string_mat);
    if(deal_string_mat.empty())
        return cv::Mat();
    if(deal_string_mat[0] != '[' || deal_string_mat.back() != ']')
        return cv::Mat();

    deal_string_mat = std::string(deal_string_mat.begin()+1, deal_string_mat.end()-1);
    std::stringstream ss(deal_string_mat);
    std::string token;
    std::vector<double> std_array;
    while(std::getline(ss, token, ',')) {
        std_array.push_back(stod(token));
    }

    cv::Mat cv_array(1, std_array.size(), CV_32F);
    for(int i = 0, n = std_array.size(); i < n; ++i) {
        cv_array.at<float>(i) = std_array[i];
    }
    return cv_array;
}

}

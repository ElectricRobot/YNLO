#include "yaml.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "util.h"

namespace ynlo {
YamlParser::YamlParser() {}

YamlParser::YamlParser(const std::string& filename) {
    Open(filename);
}

YamlParser::~YamlParser() {}

void YamlParser::Open(const std::string& filename) {
    std::ifstream ifs(filename);
    if(!ifs.is_open()) {
        std::cerr << "Cannot open the file: " << filename << std::endl;
        return;
    }

    std::string line;

    while(std::getline(ifs, line)) {
        // erase # and after #
        line = EraseTheStringAfterSpecificChar(line, '#');

        if(line.empty())
            continue;

        // find :
        auto it = std::find(line.begin(), line.end(), ':');
        if(it == line.end())
            continue;

        // find key
        std::string key(line.begin(), it);
        if(key.empty())
            continue;

        // check space
        if(isspace(key[0]))
            continue;

        key = EraseDummySpace(key);

        // find value
        std::string value(it+1, line.end());
        // cutting space
        value = EraseDummySpace(value);

        if(value.empty()) {
            // tree node, in this case only deal with the two dimensional matrix
            std::string line;
            int cols = 0, rows = 0;
            for(int i = 0; i < 2; ++i) {
                std::getline(ifs, line);
                auto it = std::find(line.begin(), line.end(), ':');
                if(it == line.end())
                    assert("error");
                std::string cols_or_rows(line.begin(), it);
                cols_or_rows = EraseDummySpace(cols_or_rows);
                if(cols_or_rows == "cols") {
                    std::string size_string(it+1, line.end());
                    cols = stoi(size_string);
                }
                else if(cols_or_rows == "rows") {
                    std::string size_string(it+1, line.end());
                    rows = stoi(size_string);
                }
            }

            std::getline(ifs, line);
            auto it = std::find(line.begin(), line.end(), ':');
            if(it == line.end())
                assert("error");

            std::string data(line.begin(), it);
            data = EraseDummySpace(data);
            if(data != "data")
                assert("error");

            data = std::string(it+1, line.end());
            data = EraseDummySpace(data);
            it = std::find(data.begin(), data.end(), ']');
            while(it == data.end()) {
                std::string line;
                std::getline(ifs, line);
                line = EraseTheStringAfterSpecificChar(line, '#');
                line = EraseDummySpace(line);
                data += line;
                it = std::find(data.begin(), data.end(), ']');
            }
            auto value_node_ptr = std::make_shared<MatNode>();
            value_node_ptr->value = String2CvMat(data, rows, cols);
            dictionary[key] = value_node_ptr;
        }
        else {
            // one dimention array cv::Mat
            if(value[0] == '[') {
                // local it, no mind
                auto it = std::find(value.begin(), value.end(), ']');
                while(it == value.end()) {
                    // local line, no mind
                    std::string line;
                    std::getline(ifs, line);
                    line = EraseTheStringAfterSpecificChar(line, '#');
                    line = EraseDummySpace(line);
                    value += line;
                    it = std::find(value.begin(), value.end(), ']');
                }

                auto value_node_ptr = std::make_shared<MatNode>();
                value_node_ptr->value = String2CvMat(value);
                dictionary[key] = value_node_ptr;
            }
            else {
                try {
                    double float_num = stod(value);
                    auto value_node_ptr = std::make_shared<NumNode>();
                    value_node_ptr->value = float_num;
                    dictionary[key] = value_node_ptr;
                }
                catch(...) {
                    auto value_node_ptr = std::make_shared<StringNode>();
                    value_node_ptr->value = value;
                    dictionary[key] = value_node_ptr;
                }
            }
        }

        std::cout << key << "->";
        dictionary[key]->PrintValue();

    }

    ifs.close();
}

}

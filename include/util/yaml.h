#ifndef YAML_H
#define YAML_H
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <opencv2/opencv.hpp>

namespace ynlo {

class YamlParser {
public:
    class BaseNode {
    public:
        BaseNode() = default;
        virtual ~BaseNode() = default;

        virtual void PrintValue() {
        }

        void operator>>(std::string& data);

        void operator>>(double& data);

        void operator>>(cv::Mat& data);
    };

    class StringNode: public BaseNode {
    public:
        void PrintValue() override {
            std::cout << value << std::endl;
        }
        std::string value;
    };

    class NumNode: public BaseNode {
    public:
        void PrintValue() override {
            std::cout << value << std::endl;
        }
        double value;
    };

    class MatNode: public BaseNode {
    public:
        void PrintValue() override {
            std::cout << value << std::endl;
        }
        cv::Mat value;
    };

    class TreeNode: public BaseNode {
    public:
        std::vector<std::string> keys;
        std::vector<std::shared_ptr<BaseNode>> children;
    };

public:
    YamlParser();
    YamlParser(const std::string& filename);
    ~YamlParser();

    void Open(const std::string& filename);

    BaseNode& operator[](const std::string& key);

    std::unordered_map<std::string, std::shared_ptr<BaseNode>> dictionary;
};

}
#endif // YAML_H

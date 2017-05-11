#ifndef PIPELINE_H
#define PIPELINE_H
#include <vector>
#include <thread>
#include <mutex>
#include <future>
#include <memory>
#include <vector>
#include <utility>
#include "Buffer.h"

namespace ynlo {

class Pipeline
{
public:
    class BaseNode {
    public:
        BaseNode() {}
        virtual ~BaseNode() {}
        virtual void MainLoop() {}
    };

    template<class T>
    class InputNode : virtual public BaseNode
    {
    public:
        InputNode() {}
        virtual ~InputNode() {}
    private:
        Buffer<T> buffer_;
    };

    template<class T>
    class OutputNode : virtual public BaseNode
    {
    public:
        OutputNode() {}
        virtual ~OutputNode() {}
    };


    template<class Input, class Output>
    class TaskNode : public InputNode<Input>, public OutputNode<Output>
    {
    public:
        TaskNode() {}
        ~TaskNode() {}
    };

public:
    Pipeline();
    ~Pipeline();
    void Init();
    void Trigger();
    void AddTask(std::shared_ptr<BaseNode> task);
    int TaskSize() const;
private:
    int size_;
    std::shared_ptr<BaseNode> root_, cur_;
    std::vector<std::thread> threads_;
    std::condition_variable cv_;
    std::mutex mutex_;
};

}
#endif // PIPELINE_H

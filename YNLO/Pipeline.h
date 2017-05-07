#ifndef PIPELINE_H
#define PIPELINE_H
#include <vector>
#include <thread>
#include <mutex>
#include <future>
#include <memory>
#include <vector>
#include <utility>

namespace ynlo {

class Pipeline
{
public:
    class BaseNode {
    public:
        BaseNode()
            :next_(nullptr) {}
        virtual ~BaseNode() {}
        virtual void MainLoop() = 0;
        std::shared_ptr<BaseNode> next_;
        std::mutex mutex_;
        std::condition_variable* cv_;
    };
    /*
    template<class InputData>
    class TaskNode : public BaseNode
    {
    public:
        TaskNode()
            :write_buffer_ptr(-1), read_buffer_ptr(-1) {}
        virtual ~TaskNode() {}
        virtual void MainLoop();

        void WriteToBuffers(const InputData& data) {
            write_buffer_ptr = (write_buffer_ptr+1)%buffer_size_;
            buffers_[write_buffer_ptr] = data;
            read_buffer_ptr = write_buffer_ptr;
        }

        void WriteToBuffers(InputData&& data) {
            write_buffer_ptr = (write_buffer_ptr+1)%buffer_size_;
            buffers_[write_buffer_ptr] = std::move(data);
            read_buffer_ptr = write_buffer_ptr;
        }

        InputData* ReadFromBuffers() {
            if(read_buffer_ptr == -1)
                return nullptr;
            return buffers_[read_buffer_ptr];
        }

    protected:
        int buffer_size_;
        int write_buffer_ptr, read_buffer_ptr;
        std::vector<InputData> buffers_;
    };
    */
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
};

}
#endif // PIPELINE_H

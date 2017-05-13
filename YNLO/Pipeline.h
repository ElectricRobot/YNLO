#ifndef PIPELINE_H
#define PIPELINE_H
#include <vector>
#include <thread>
#include <mutex>
#include <future>
#include <memory>
#include <vector>
#include <atomic>
#include <utility>
#include "Buffer.h"

namespace ynlo {

class Pipeline
{
public:
    class BaseNode {
    public:
        friend class Pipeline;
        BaseNode() {}
        virtual ~BaseNode() {}
        virtual void MainLoop() {}
        bool CheckStopRequest() {
            return *stop_flag_;
        }

    protected:
        void SetCV(std::condition_variable* cv) {cv_ = cv;}
        void SetMutex(std::mutex* mutex) {mutex_ = mutex;}
        void SetStopFlag(std::atomic<bool>* stop_flag) {stop_flag_ = stop_flag;}

        std::condition_variable* cv_;
        std::mutex* mutex_;
        std::atomic<bool>* stop_flag_;
    };

    template<class T>
    class InputNode : virtual public BaseNode
    {
    public:
        InputNode() {}
        virtual ~InputNode() {}
        void SetBufferSize(int size) {
            buffer_.Reset(size);
        }

        void WriteToBuffer(const T& data) {
            buffer_.WriteToBuffer(data);
        }

        void WriteToBuffer(T&& data) {
            buffer_.WriteToBuffer(std::move(data));
        }

        T* ReadFromBuffer() {
            return buffer_.ReadFromBuffer();
        }

    protected:
        Buffer<T> buffer_;
    };

    template<class T>
    class OutputNode : virtual public BaseNode
    {
    public:
        OutputNode()
            :next_(nullptr)
        {}
        virtual ~OutputNode() {}

        virtual T Process() {
            return T();
        }

        void LinkNextInputNode(std::shared_ptr<InputNode<T> > next) {
            next_ = next;
        }

    protected:
        std::shared_ptr<InputNode<T> > next_;
    };


    template<class Input, class Output>
    class TaskNode : public InputNode<Input>, public OutputNode<Output>
    {
    public:
        TaskNode() {}
        ~TaskNode() {}

        virtual void MainLoop() override
        {
            while(1) {
                std::unique_lock<std::mutex> lock(*this->mutex_);
                this->cv_->wait(lock);
                lock.unlock();

                Input* input_data_ptr = this->ReadFromBuffer();
                if(input_data_ptr && this->next_)
                    this->next_->WriteToBuffer(Process(*input_data_ptr));

                if(this->CheckStopRequest())
                    break;
            }
        }

        virtual Output Process(const Input& input_data)
        {
            return Output();
        }
    };

public:
    Pipeline()
        :size_(0), stop_flag_(false) {}
    ~Pipeline()
    {
        stop_flag_ = true;
        for(auto& it : threads_) {
            it.join();
        }
    }

    void Init() {
        threads_.resize(size_);
        for(int i = 0; i < size_; ++i) {
            threads_[i] = std::thread([&]
            {
                nodes_[i]->SetCV(&cv_);
                nodes_[i]->SetMutex(&mutex_);
                nodes_[i]->SetStopFlag(&stop_flag_);
                nodes_[i]->MainLoop();
            });
        }
    }

    void Trigger() {
        cv_.notify_all();
    }

    void AddTask(std::shared_ptr<BaseNode> task) {
        nodes_.push_back(task);
        ++size_;
    }

    int TaskSize() {
        return size_;
    }
private:
    int size_;
    std::vector<std::shared_ptr<BaseNode> >nodes_;
    std::vector<std::thread> threads_;
    std::condition_variable cv_;
    std::mutex mutex_;
    std::atomic<bool> stop_flag_;
};

}
#endif // PIPELINE_H

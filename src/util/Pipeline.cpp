#include "Pipeline.h"

namespace ynlo {

Pipeline::Pipeline() :
        size_(0), stop_flag_(false) {
}

Pipeline::~Pipeline() {
    stop_flag_ = true;
    for (auto& it : threads_) {
        it.join();
    }
}

void Pipeline::Init() {
    threads_.resize(size_);
    for (int i = 0; i < size_; ++i) {
        threads_[i] = std::thread([&]
        {
            nodes_[i]->SetCV(&cv_);
            nodes_[i]->SetMutex(&mutex_);
            nodes_[i]->SetStopFlag(&stop_flag_);
            nodes_[i]->MainLoop();
        });
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void Pipeline::Trigger() {
    cv_.notify_all();
}

void Pipeline::AddTask(std::shared_ptr<BaseNode> task) {
    nodes_.push_back(task);
    ++size_;
}

int Pipeline::TaskSize() {
    return size_;
}
}

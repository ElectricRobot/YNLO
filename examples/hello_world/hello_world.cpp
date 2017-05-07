#include <iostream>
#include <thread>
#include <mutex>
#include <future>
#include <chrono>
#include <random>
#include <cassert>
#include <queue>
using namespace std;

class Task{
public:
    Task(int id, int mu, int sigma)
        :id_(id), ave_time_(mu), std_time_(sigma)
    {
        assert(mu > 0 && sigma > 0);
        gen_ = mt19937(rd_());
        d_ = normal_distribution<>(ave_time_, std_time_);
    }
    ~Task() {}

    void Input(int problem_id) {
        task_time_ = std::abs(d_(gen_));
        this_thread::sleep_for(chrono::milliseconds(task_time_));
        cout << "Task" << id_ << " spent " << task_time_ << " (ms), deal " << problem_id << " problem" << endl;
        workQueue_.push(problem_id);
    }

    void Input(queue<int>& workQueue) {
        if(workQueue.empty())
            return;
        int problem_id = workQueue.front();
        workQueue.pop();

        task_time_ = std::abs(d_(gen_));
        this_thread::sleep_for(chrono::milliseconds(task_time_));
        cout << "Task" << id_ << " spent " << task_time_ << " (ms), deal " << problem_id << " problem" << endl;
        workQueue_.push(problem_id);
    }

    queue<int>& Output() {
        return workQueue_;
    }

private:
    int id_;
    int ave_time_;
    int std_time_;
    int task_time_;
    random_device rd_;
    mt19937 gen_;
    normal_distribution<> d_;
    queue<int> workQueue_;
};

class Pipeline {
public:
    Pipeline() {}
    ~Pipeline() {}

    void Trigger() {
         while(1) {
             this_thread::sleep_for(chrono::milliseconds(trigger_time_));
             cout << "----------------------------------------------------------------" << endl;
             cv_.notify_all();
         }
    }

    void setTriggerTime(int time) {
        trigger_time_ = time;
    }

    mutex& GetMutex() {
        return mutex_;
    }

    condition_variable& GetCv() {
        return cv_;
    }

private:
    int trigger_time_;
    mutex mutex_;
    condition_variable cv_;
};

int main(int argc, char *argv[])
{
    Pipeline pipeline;
    pipeline.setTriggerTime(16);
    Task task0(0, 10, 1), task1(1, 9, 1), task2(2, 9, 1);

    thread t0([&] {
        for(int i = 0;; ++i) {
            unique_lock<mutex> lock(pipeline.GetMutex());
            pipeline.GetCv().wait(lock);
            lock.unlock();
            task0.Input(i);
        }
    });

    thread t1([&] {
        while(1) {
            unique_lock<mutex> lock(pipeline.GetMutex());
            pipeline.GetCv().wait(lock);
            lock.unlock();
            task1.Input(task0.Output());
        }
    });

    thread t2([&] {
        while(1) {
            unique_lock<mutex> lock(pipeline.GetMutex());
            pipeline.GetCv().wait(lock);
            lock.unlock();
            task2.Input(task1.Output());
        }
    });

    pipeline.Trigger();
    return 0;
}

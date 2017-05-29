/*
 * main.cpp
 *
 *  Created on: May 29, 2017
 *      Author: liu
 */
#include <iostream>
#include <chrono>
#include <memory>
#include "Pipeline.h"
using namespace std;

class Task1 : public ynlo::Pipeline::TaskNode<int , int>
{
	int Process(const int& input_data) override {
		std::cout << "Task1 process data " << input_data << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(160));
		return input_data;
	}
};

class Task2 : public ynlo::Pipeline::TaskNode<int, float>
{
	float Process(const int& input_data) override {
		std::cout << "Task2 process data " << input_data << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		return input_data*1.2f;
	}
};

class Task3 : public ynlo::Pipeline::InputNode<float>
{
	void MainLoop() {
		while(1) {
            std::unique_lock<std::mutex> lock(*this->mutex_);
            this->cv_->wait(lock);
            lock.unlock();

            auto it = this->ReadFromBuffer();
            if(it)
            	std::cout << *it << std::endl;

            if(this->CheckStopRequest())
                break;
		}
	}
};

int main() {

	std::shared_ptr<Task1> task1 = std::make_shared<Task1>();
	std::shared_ptr<Task2> task2 = std::make_shared<Task2>();
	std::shared_ptr<Task3> output = std::make_shared<Task3>();
	task1->LinkNextInputNode(task2);
	task2->LinkNextInputNode(output);

	ynlo::Pipeline pipeline;
	pipeline.AddTask(task1);
	pipeline.AddTask(task2);
	pipeline.AddTask(output);
	pipeline.Init();

	for(int i = 0; i < 100; ++i) {
		std::cout << "main process data " << i << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
		task1->WriteToBuffer(i);
		pipeline.Trigger();
	}

	return 0;
}

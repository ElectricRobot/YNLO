#ifndef BUFFER_H
#define BUFFER_H
#include <vector>
#include <mutex>

namespace ynlo {
template <class T>
class Buffer
{
public:
    Buffer(int size = 2)
        :size_(size), read_buffer_ptr(-1), write_buffer_ptr(-1)
    {
        buffers_.resize(size_);
    }

    ~Buffer() {}

    void Reset(int size) {
        size_ = size;
        buffers_.resize(size_);
        read_buffer_ptr = write_buffer_ptr = -1;
    }

    void WriteToBuffer(const T& data) {
        write_buffer_ptr = (write_buffer_ptr+1)%size_;
        buffers_[write_buffer_ptr] = data;
        read_buffer_ptr = write_buffer_ptr;
    }

    void WriteToBuffer(T&& data) {
        write_buffer_ptr = (write_buffer_ptr+1)%size_;
        buffers_[write_buffer_ptr] = std::move(data);
        read_buffer_ptr = write_buffer_ptr;
    }

    T* ReadFromBuffer() {
        // unique lock is require? maybe not
        return (read_buffer_ptr == -1)? nullptr:buffers_[read_buffer_ptr];
    }

private:
    int size_;
    int read_buffer_ptr, write_buffer_ptr;
    std::vector<T> buffers_;
};
}
#endif // BUFFER_H

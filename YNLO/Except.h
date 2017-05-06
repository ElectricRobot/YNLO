#ifndef EXCEPT_H
#define EXCEPT_H
#include <stdexcept>

namespace ynlo {

class ynlo_runtime_error : public std::runtime_error {
public:
    ynlo_runtime_error(const char* w) : std::runtime_error(w) {}
};

}
#endif // EXCEPT_H

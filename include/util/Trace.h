/*
 * Trace.h
 *
 *  Created on: May 30, 2017
 *      Author: liu
 */

#ifndef INCLUDE_UTIL_TRACE_H_
#define INCLUDE_UTIL_TRACE_H_

namespace ynlo {

namespace Tracer {

void Init();

void TraceBegin(const char* name);

void TraceEnd();

void Close();

}

class ScopedTrace {
public:
    ScopedTrace(const char* name);
    ~ScopedTrace();
};

} /* namespace ynlo */

#endif /* INCLUDE_UTIL_TRACE_H_ */

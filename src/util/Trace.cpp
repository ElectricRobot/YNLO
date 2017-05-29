/*
 * Trace.cpp
 *
 *  Created on: May 30, 2017
 *      Author: liu
 */
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include "Trace.h"

namespace ynlo {

namespace Tracer {

static int sTraceFD = -1;

void Init() {
    const char* const traceFileName = "/sys/kernel/debug/tracing/trace_marker";

    sTraceFD = open(traceFileName, O_WRONLY);

    if(sTraceFD == -1) {
        std::cerr << "error opening trace file: " << strerror(errno) << " (" << errno << ")" << std::endl;
    }
}

void TraceBegin(const char* name) {
    char buf[1024];
    size_t len = snprintf(buf, 1024, "B|%d|%s", getpid(), name);
    write(sTraceFD, buf, len);
}

void TraceEnd() {
    char buf = 'E';
    write(sTraceFD, &buf, 1);
}

void Close() {
    if(sTraceFD != -1)
        close(sTraceFD);
}

} /* namespace Tracer */

ScopedTrace::ScopedTrace(const char* name) {
    Tracer::TraceBegin(name);
}

ScopedTrace::~ScopedTrace() {
    Tracer::TraceEnd();
}

} /* namespace ynlo */

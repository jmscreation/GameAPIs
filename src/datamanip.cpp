#include "datamanip.h"
namespace gapi {
    DataManipulator::DataManipulator(): readonly(false), error(false), pos(0), length(0), wdata( std::make_unique<std::stringstream>() ) {}
    DataManipulator::DataManipulator(const char* data, size_t length):
                                        readonly(true), error(false), pos(0), length(length), rdata(data) {}
}
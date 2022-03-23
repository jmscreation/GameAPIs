#include "datamanip.h"
namespace gapi {
    DataManipulator::DataManipulator(): readonly(false), error(false), pos(0), length(0), wdata( std::make_unique<std::stringstream>() ) {}
    DataManipulator::DataManipulator(const char* data, size_t length):
                                        readonly(true), error(false), pos(0), length(length), rdata(data) {}

    bool DataManipulator::writeData(const std::string& rval) {
        if(readonly) return false;
        int sz = rval.size() + 1; // null terminated string

        return wdata->write(rval.data(), sz - 1).good() &&
                wdata->write("\0", 1).good();
    };

    bool DataManipulator::readData(std::string& rval, bool peek) {
        if(!readonly) return false;

        size_t sz = strnlen(rdata + pos, length - pos);

        if(pos + sz >= length){
            error = true;
            return false;
        }

        rval.assign(rdata + pos, sz);

        if(!peek) pos += sz + 1;
        return true;
    };
}
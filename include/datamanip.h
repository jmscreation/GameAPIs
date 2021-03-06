#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <memory>
#include <numeric>
#include <type_traits>

namespace gapi {

    class DataManipulator {

        bool readonly, error;
        size_t pos, length;

        union {
            const char* rdata;
            std::unique_ptr<std::stringstream> wdata;
        };

    public:

        inline const char* getCurPointer() {
            if(!readonly) return nullptr;
            return rdata + pos;
        }

        inline bool ignoreBytes(size_t sz) {
            if(pos + sz - 1 >= length){
                error = true;
                return false;
            }
            pos += sz;
            return true;
        }

        template<class T>
        bool readData(T& rval, bool peek=false) {
            if(!readonly) return false;

            int sz = sizeof(rval);

            if(pos + sz - 1 >= length){
                error = true;
                return false;
            }

            memcpy(reinterpret_cast<char*>(&rval), rdata + pos, sz);
            
            if(!peek) pos += sz;
            return true;
        };

        bool readData(std::string& rval, bool peek=false);

        template<class T>
        bool writeData(const T& rval) {
            if(readonly) return false;
            int sz = sizeof(rval);

            return wdata->write(reinterpret_cast<const char*>(&rval), sz).good();
        };

        bool writeData(const std::string& rval);

        inline bool exportData(std::stringstream& stream) { // append data to stream
            if(readonly) return false;

            return (stream << wdata->rdbuf()).good();
        }

        DataManipulator(); // writer
        DataManipulator(const char* data, size_t length); // reader

        virtual ~DataManipulator() {}
    };

}
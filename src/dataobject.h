#pragma once

#include "datamanip.h"

namespace gapi {
    // inherit from this class to gain read/write access to a save file
    class enable_data_object {
    public:
        virtual bool onSaveObject(DataManipulator& datamanip) = 0; // called on object save - return success&continue
        virtual bool onLoadObject(DataManipulator& datamanip) = 0; // called on object load - return success&continue
    };
}
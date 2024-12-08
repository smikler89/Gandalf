#pragma once

#include "../util/manager.h"

namespace Gandalf {
    enum ECompressionType {
        Binary = 1,
        Json = 2,
        PrettyJson = 3,
        ZLib = 4,
        LZ4 = 5, // not supported yet
    };

    // each profile stored in a separate file (designed for debug and unit-tests purposes)
    std::shared_ptr<IProfileManager> CreateFileProfileManager(
        const std::string& folderPath,
        ECompressionType ct
    );
};

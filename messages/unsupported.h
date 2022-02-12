#pragma once

#include <fstream>
#include "../types.h"

struct UnsupportedMessageType {
    static void skip(std::ifstream& file, u64 n) {
        for (int i = 0; i < n; i++) {
            file.get();
        }
    }
};

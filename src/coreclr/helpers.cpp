#include "host-coreclr/helpers.h"
#include <sstream>
#include <vector>
#include <iostream>

#ifdef _WIN32
#define TPA_SEPERATOR ";"
#else
#define TPA_SEPERATOR ":"
#endif

namespace hostcoreclr {
    std::string build_tpa_list(const std::filesystem::path& path) {
        std::stringstream stream;

        build_tpa_list(path, stream);

        return stream.str().substr(1);
    }

    std::string build_tpa_list(const std::vector<std::filesystem::path>& paths) {
        std::stringstream stream;

        for(auto& path : paths) {
            build_tpa_list(path, stream);
        }

        return stream.str().substr(1);
    }

    void build_tpa_list(const std::filesystem::path& root_path, std::stringstream& stream) {
        auto it = std::filesystem::recursive_directory_iterator(root_path);

        for(auto& entry : it) {
            if(entry.is_regular_file() == false || entry.path().extension() != ".dll") {
                continue;
            }

            stream << TPA_SEPERATOR << entry.path().string();
        }
    }
}
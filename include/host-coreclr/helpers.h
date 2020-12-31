#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <sstream>

namespace hostcoreclr {
    std::string build_tpa_list(const std::filesystem::path& path);
    std::string build_tpa_list(const std::vector<std::filesystem::path>& paths);
    void build_tpa_list(const std::filesystem::path& root_path, std::stringstream& stream);
}
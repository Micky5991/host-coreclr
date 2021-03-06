#pragma once

#include <filesystem>
#include <vector>
#include "coreclrhost.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#ifdef _WIN32
typedef HMODULE coreclr_library_t;
#else
typedef void* coreclr_library_t;
#endif

class CoreClr {

    coreclr_initialize_ptr _coreclr_initialize;
    coreclr_create_delegate_ptr _coreclr_create_delegate;
    coreclr_execute_assembly_ptr _coreclr_execute_assembly;
    coreclr_shutdown_2_ptr _coreclr_shutdown;

    void* _coreclr_host_handle;
    unsigned int _coreclr_domain_id;

    bool load_library(std::filesystem::path library_path);
    bool initialize_host(const std::filesystem::path& runtime_path, const std::filesystem::path& app_path,
                         const std::vector<std::filesystem::path>& additional_paths = std::vector<std::filesystem::path>(),
                         const std::vector<std::filesystem::path>& additional_native_paths = std::vector<std::filesystem::path>());

public:

    CoreClr();
    ~CoreClr();

    bool initialize(const std::filesystem::path& runtime_path, const std::filesystem::path& plugin_path,
                    const std::vector<std::filesystem::path>& additional_native_paths = std::vector<std::filesystem::path>());
    bool start(const std::filesystem::path& assembly_path, int argc, const char** argv);

    int stop();

};




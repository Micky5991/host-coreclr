#include <iostream>
#include <string>

#include "host-coreclr/core_clr.h"
#include "host-coreclr/helpers.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

CoreClr::CoreClr() {
    _coreclr_host_handle = nullptr;
    _coreclr_domain_id = 0;

    _coreclr_initialize = nullptr;
    _coreclr_execute_assembly = nullptr;
    _coreclr_create_delegate = nullptr;
    _coreclr_shutdown = nullptr;
}

CoreClr::~CoreClr() {
    _coreclr_host_handle = nullptr;
    _coreclr_domain_id = 0;

    _coreclr_initialize = nullptr;
    _coreclr_execute_assembly = nullptr;
    _coreclr_create_delegate = nullptr;
    _coreclr_shutdown = nullptr;
}

bool CoreClr::initialize(const std::filesystem::path& runtime_path, const std::filesystem::path& plugin_path,
                         const std::vector<std::filesystem::path>& additional_native_paths) {
    return load_library(runtime_path) && initialize_host(runtime_path, plugin_path, additional_native_paths);
}

bool CoreClr::start(const std::filesystem::path& assembly_path, int argc, const char** argv) {
    if(std::filesystem::exists(assembly_path) == false) {
        std::cerr << "Could not find assembly " << assembly_path.string() << std::endl;

        return false;
    }

    unsigned int exit_code;

    std::string assembly_location = assembly_path.string();

    int hresult = _coreclr_execute_assembly(
            _coreclr_host_handle,
            _coreclr_domain_id,
            argc,
            argv,
            assembly_location.c_str(),
            &exit_code);

    return hresult == 0;
}

int CoreClr::stop() {
    return _coreclr_shutdown(_coreclr_host_handle, _coreclr_domain_id, nullptr);
}

bool CoreClr::load_library(std::filesystem::path library_path) {
    coreclr_library_t library;

#ifdef _WIN32
    library_path /= "coreclr.dll";
    if(std::filesystem::exists(library_path) == false) {
        std::cerr << "Unable to find " << library_path << std::endl;

        return false;
    }

    std::string library_location = library_path.string();

    library = LoadLibraryExA(library_location.c_str(), nullptr, 0);
#else
    library_path /= "libcoreclr.so";
    if(std::filesystem::exists(library_path) == false) {
        std::cerr << "Unable to find " << library_path << std::endl;

        return false;
    }

    std::string library_location = library_path.string();

    return dlopen(library_location.c_str(), RTLD_NOW | RTLD_LOCAL);
#endif
    if(library == nullptr) {
        std::cerr << "Unable to load library " << library_path << std::endl;

        return false;
    }

#if WIN32
    _coreclr_initialize = (coreclr_initialize_ptr) GetProcAddress(library, "coreclr_initialize");
    _coreclr_create_delegate = (coreclr_create_delegate_ptr) GetProcAddress(library, "coreclr_create_delegate");
    _coreclr_execute_assembly = (coreclr_execute_assembly_ptr) GetProcAddress(library, "coreclr_execute_assembly");
    _coreclr_shutdown = (coreclr_shutdown_2_ptr) GetProcAddress(library, "coreclr_shutdown_2");
#elif LINUX
    _coreclr_initialize = (coreclr_initialize_ptr) dlsym(coreClr, "coreclr_initialize");
    _coreclr_create_delegate = (coreclr_create_delegate_ptr) dlsym(coreClr, "coreclr_create_delegate");
    _coreclr_execute_assembly = (coreclr_execute_assembly_ptr) dlsym(coreClr, "coreclr_execute_assembly");
    _coreclr_shutdown = (coreclr_shutdown_2_ptr) dlsym(coreClr, "coreclr_shutdown_2");
#endif

    return true;
}

bool CoreClr::initialize_host(const std::filesystem::path& runtime_path, const std::filesystem::path& app_path,
                              const std::vector<std::filesystem::path>& additional_paths,
                              const std::vector<std::filesystem::path>& additional_native_paths) {

    std::vector<std::filesystem::path> platform_paths({ runtime_path, app_path });
    std::string trusted_assemblies = hostcoreclr::build_tpa_list(platform_paths);

    std::vector<std::filesystem::path> search_directories = additional_paths;
    search_directories.insert(search_directories.end(), platform_paths.begin(), platform_paths.end());

    std::vector<std::filesystem::path> native_search_directories = additional_native_paths;
    native_search_directories.insert(native_search_directories.end(), platform_paths.begin(), platform_paths.end());

    std::string search_location = hostcoreclr::build_tpa_list(search_directories);
    std::string native_search_location = hostcoreclr::build_tpa_list(native_search_directories);

    const char* propertyKeys[] = {
            "TRUSTED_PLATFORM_ASSEMBLIES",
            "APP_PATHS",
            "APP_NI_PATHS",
            "NATIVE_DLL_SEARCH_DIRECTORIES",
    };

    const char* propertyValues[] = {
            trusted_assemblies.c_str(),
            search_location.c_str(),
            native_search_location.c_str(),
            native_search_location.c_str()
    };

    std::string runtime_location = runtime_path.string();
    int hresult = _coreclr_initialize(
            runtime_location.c_str(),
            "Hosted CoreCLR AppDomain",
            sizeof (propertyKeys) / sizeof(propertyKeys[0]),
            propertyKeys,
            propertyValues,
            &_coreclr_host_handle,
            &_coreclr_domain_id);

    return hresult == 0;
}
#include <filesystem>

#include <host-coreclr/core_clr.h>

int main(int argc, const char** argv) {
    auto core_clr = new CoreClr();

    std::filesystem::path plugin_path = std::filesystem::current_path();
    std::filesystem::path runtime_path = std::filesystem::current_path();

    plugin_path /= "plugins";
    runtime_path /= "runtime";

    std::filesystem::path assembly_path = plugin_path;
    assembly_path /= "NetApplication.dll";

    if(core_clr->initialize(runtime_path, plugin_path) == false) {
        return -1;
    }

    if(core_clr->start(assembly_path, argc, argv) == false) {
        return -1;
    }

    int exit_code = core_clr->stop();

    delete core_clr;

    return exit_code;
}
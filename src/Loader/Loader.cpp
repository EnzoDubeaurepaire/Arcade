/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** Loader
*/

#include "Loader.hpp"

DynamicLibrary::DynamicLibrary(const std::string& path) {
    this->handle = dlopen(path.c_str(), RTLD_NOW);
    if (!this->handle)
        throw DynamicLibraryException(dlerror());
}

DynamicLibrary::~DynamicLibrary() {
    if (this->handle)
        dlclose(this->handle);
}

void *DynamicLibrary::getSymbol(const std::string& symbolName) const {
    dlerror();
    void *symbol = dlsym(this->handle, symbolName.c_str());
    const char *error = dlerror();
    if (error || symbol == nullptr)
        return nullptr;
    return symbol;
}

bool DynamicLibrary::isLoaded() {
    struct link_map *map;
    if (dlinfo(this->handle, RTLD_DI_LINKMAP, &map) == 0)
        return true;
    this->handle = nullptr;
    return false;
}


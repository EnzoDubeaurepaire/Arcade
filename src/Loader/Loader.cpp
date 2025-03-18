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

template <typename T>
T DynamicLibrary::getSymbol(const std::string& symbolName) {
    dlerror();
    void *symbol = dlsym(this->handle, symbolName.c_str());
    if (const char *error = dlerror())
        throw DynamicLibraryException(error);
    return reinterpret_cast<T>(symbol);
}

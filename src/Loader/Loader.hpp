/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** Loader
*/

#ifndef LOADER_H
#define LOADER_H
#include <any>
#include <iostream>
#include <dlfcn.h>
#include <memory>

class DynamicLibrary {
public:
    // Exception for dynamic library
    class DynamicLibraryException : public std::exception {
    public:
        explicit DynamicLibraryException(const std::string& what) : message("DynamicLibrary error : " + what) {}
        const char* what() const noexcept override {return this->message.c_str();}
    private:
        std::string message;
    };

    // Constructor
    explicit DynamicLibrary(const std::string& path);
    DynamicLibrary() {handle = nullptr;};
    // Destructor
    ~DynamicLibrary();

    // Symbol getter
    void *getSymbol(const std::string& symbolName) const;

    bool isLoaded();

private:
    // Handle
    void *handle;
};

#endif

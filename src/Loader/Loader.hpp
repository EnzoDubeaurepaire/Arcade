/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** Loader
*/

#ifndef LOADER_H
#define LOADER_H
#include <iostream>
#include <dlfcn.h>

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
    // Destructor
    ~DynamicLibrary();

    // Symbol getter
    template <typename T>
    T getSymbol(const std::string& symbolName);

private:
    // Handle
    void *handle;
};

#endif

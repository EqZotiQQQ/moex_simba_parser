#pragma once

#include <exception>

class FileNotFoundException: public std::exception {
    const char* what() const noexcept override
    {
        return "File not found exception";
    }
};

class InvalidMagicNumberException: public std::exception {
    const char* what() const noexcept override
    {
        return "Invalid magic number exception";
    }
};

class BadMessageTypeException: public std::exception {
    const char* what() const noexcept override
    {
        return "Invalid message type";
    }
};

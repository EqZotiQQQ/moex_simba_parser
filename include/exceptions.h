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

class UnsupportedVersionException: public std::exception {
    const char* what() const noexcept override
    {
        return "UnsupportedVersionException";
    }
};

class BadHeaderLengthException: public std::exception {
    const char* what() const noexcept override
    {
        return "BadHeaderLengthException";
    }
};

class UnsupportedDiffServException: public std::exception {
    const char* what() const noexcept override
    {
        return "UnsupportedDiffServException";
    }
};

class UnsupportedProtocolException: public std::exception {
    const char* what() const noexcept override
    {
        return "UnsupportedProtocolException";
    }
};

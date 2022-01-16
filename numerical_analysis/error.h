#pragma once

#include <iostream>
#include <stdexcept>
#include <string>

// from numcpp Core/Internal/Error.hpp

#define THROW_INVALID_ARGUMENT_ERROR(msg) ht::error::throwError<std::invalid_argument>(__FILE__, __func__, __LINE__, msg)
#define THROW_RUNTIME_ERROR(msg) ht::error::throwError<std::runtime_error>(__FILE__, __func__, __LINE__, msg)
#define THROW_OUT_OF_RANGE_ERROR(msg) ht::error::throwError<std::out_of_range>(__FILE__, __func__, __LINE__, msg)

namespace ht
{
namespace error
{
    template<typename ErrorType>
    void throwError(const std::string& file, const std::string& function, size_t line, const std::string& msg = "")
    {
        std::string errMsg = "File: " + file + 
                                       "\n\tFunction: " + function + 
                                       "\n\tLine: " + std::to_string(line) + 
                                       "\n\tError: " + msg;
        std::cerr << errMsg;
        throw ErrorType(errMsg);
    }
} // namespace error
} // namespace ht
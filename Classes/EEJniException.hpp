//
//  JniException.hpp
//  ee-library
//
//  Created by enrevol on 10/31/15.
//
//

#ifndef EE_LIBRARY_JNI_EXCEPTION_HPP
#define EE_LIBRARY_JNI_EXCEPTION_HPP

#include "EEMacro.hpp"

#include <exception>
#include <string>

namespace_ee_begin
class JniException final : public std::exception {
public:
    /**
     * Constructor (C++ STL strings).
     * @param message The error message.
     */
    explicit JniException(const std::string& message);
    
    /**
     * Constructor (C strings).
     * @param message C-style string error message.
     */
    explicit JniException(const char* message);
    
    /**
     * Returns a pointer to the (constant) error description.
     * @return A pointer to a @c const @c char*.
     */
    virtual const char* what() const throw ();
    
private:
    /**
     * Error message.
     */
    std::string _message;
};
namespace_ee_end

#endif /* EE_LIBRARY_JNI_EXCEPTION_HPP */

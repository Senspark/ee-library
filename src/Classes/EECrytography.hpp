//
//  EECrytography.hpp
//  ee-library
//
//  Created by enrevol on 10/27/15.
//
//

/*
 Credit here:
 
 Copyright (c) 2011, Micael Hildenborg
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.
 * Neither the name of Micael Hildenborg nor the
 names of its contributors may be used to endorse or promote products
 derived from this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY Micael Hildenborg ''AS IS'' AND ANY
 EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL Micael Hildenborg BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef EE_LIBRARY_CRYTOGRAPHY_HPP
#define EE_LIBRARY_CRYTOGRAPHY_HPP

#include "EEMacro.hpp"

#include <string>

namespace_ee_begin
/**
 * @param input is the string to be hashed.
 * @return SHA-1 hashed hexadecimal string.
 */
std::string generateSha1(const std::string& input);

/**
 * @param input is the string to be hashed.
 * @return 32bit CRC hashed hexadecimal string.
 */
std::string generateCrc(const std::string& input);
namespace_ee_end

#endif /* EECrytography_h */

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

#ifndef EE_LIBRARY_SHA1_H
#define EE_LIBRARY_SHA1_H

#include "EEMacro.hpp"

#include <string>

#define namespace_sha1_begin NAMESPACE_BEGIN(sha1)
#define namespace_sha1_end NAMESPACE_END(sha1)

namespace_ee_begin
namespace_sha1_begin
/**
 * @param src points to any kind of data to be hashed.
 * @param byteLength is the number of bytes to hash from the src pointer.
 * @param hash should points to a buffer of at least 20 bytes of size for storing the SHA-1 result in.
 */
void calc(const void* src, const int byteLength, unsigned char* hash);

/**
 * @param hash is 20 bytes of SHA-1 hash. This is the same data that is the result from the calc function.
 * @param hexString should point to a buffer of at least 41 bytes of size for storing the hexadecimal representation of the hash. A zero will be written at position 40, so the buffer will be a valid zero ended string.
 */
void convertByteToHexString(const unsigned char* hash, char* hexString);

/**
 * @param input is the string to be hashed.
 * @return SHA-1 hashed hexadecimal string.
 */
std::string generate(const std::string& input);
namespace_sha1_end
namespace_ee_end

#endif // EE_LIBRARY_SHA1_H

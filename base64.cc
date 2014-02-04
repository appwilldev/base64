// Copyright Joyent, Inc. and other Node contributors.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to permit
// persons to whom the Software is furnished to do so, subject to the
// following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN
// NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
// USE OR OTHER DEALINGS IN THE SOFTWARE.
#include "base64.hpp"
//// Base 64 ////
namespace base64{
void encode(const char * src,size_t src_l, up<char []> & dst, size_t & dst_l){
    assert(dst.get()==nullptr);
    dst_l = get_base64_encoded_size(src_l);
    dst.reset(new char[dst_l]);
    encode(src,src_l,dst.get());
}
void decode(const char * src,size_t src_l, up<char []> & dst, size_t & dst_l){
    assert(dst.get()==nullptr);
    dst_l = get_base64_decoded_size(src,src_l);
    dst.reset(new char[dst_l]);
    decode(src,src + src_l,dst.get());
}
}

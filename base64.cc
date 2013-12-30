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
int foo(int a){
  return a;
}
namespace base64{
  namespace{
    #define base64_encoded_size(size) ((size + 2 - ((size + 2) % 3)) / 3 * 4)


    // Doesn't check for padding at the end.  Can be 1-2 bytes over.
    inline size_t base64_decoded_size_fast(size_t size) {
      size_t remainder = size % 4;

      size = (size / 4) * 3;
      if (remainder) {
        if (size == 0 && remainder == 1) {
          // special case: 1-byte input cannot be decoded
          size = 0;
        } else {
          // non-padded input, add 1 or 2 extra bytes
          size += 1 + (remainder == 3);
        }
      }

      return size;
    }

    static inline size_t base64_decoded_size(const char* src, size_t size) {
      if (size == 0)
        return 0;

      if (src[size - 1] == '=')
        size--;
      if (size > 0 && src[size - 1] == '=')
        size--;

      return base64_decoded_size_fast(size);
    }


    // supports regular and URL-safe base64
    static const int unbase64_table[] =
      { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -2, -1, -1, -2, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, 62, -1, 63,
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
        -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, 63,
        -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
        41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
      };
    #define unbase64(x) unbase64_table[(uint8_t)(x)]


    size_t base64_decode(char *buf,
                        size_t len,
                        const char *src,
                        const size_t srcLen) {
      char a, b, c, d;
      char* dst = buf;
      char* dstEnd = buf + len;
      const char* srcEnd = src + srcLen;

      while (src < srcEnd && dst < dstEnd) {
        int remaining = srcEnd - src;

        while (unbase64(*src) < 0 && src < srcEnd) src++, remaining--;
        if (remaining == 0 || *src == '=') break;
        a = unbase64(*src++);

        while (unbase64(*src) < 0 && src < srcEnd) src++, remaining--;
        if (remaining <= 1 || *src == '=') break;
        b = unbase64(*src++);

        *dst++ = (a << 2) | ((b & 0x30) >> 4);
        if (dst == dstEnd) break;

        while (unbase64(*src) < 0 && src < srcEnd) src++, remaining--;
        if (remaining <= 2 || *src == '=') break;
        c = unbase64(*src++);

        *dst++ = ((b & 0x0F) << 4) | ((c & 0x3C) >> 2);
        if (dst == dstEnd) break;

        while (unbase64(*src) < 0 && src < srcEnd) src++, remaining--;
        if (remaining <= 3 || *src == '=') break;
        d = unbase64(*src++);

        *dst++ = ((c & 0x03) << 6) | (d & 0x3F);
      }

      return dst - buf;
    }
    size_t base64_encode(const char* src,
                                size_t slen,
                                char* dst,
                                size_t dlen) {
      // We know how much we'll write, just make sure that there's space.
      assert(dlen >= base64_encoded_size(slen) &&
          "not enough space provided for base64 encode");

      dlen = base64_encoded_size(slen);

      unsigned a;
      unsigned b;
      unsigned c;
      unsigned i;
      unsigned k;
      unsigned n;

      static const char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                  "abcdefghijklmnopqrstuvwxyz"
                                  "0123456789+/";

      i = 0;
      k = 0;
      n = slen / 3 * 3;

      while (i < n) {
        a = src[i + 0] & 0xff;
        b = src[i + 1] & 0xff;
        c = src[i + 2] & 0xff;

        dst[k + 0] = table[a >> 2];
        dst[k + 1] = table[((a & 3) << 4) | (b >> 4)];
        dst[k + 2] = table[((b & 0x0f) << 2) | (c >> 6)];
        dst[k + 3] = table[c & 0x3f];

        i += 3;
        k += 4;
      }

      if (n != slen) {
        switch (slen - n) {
          case 1:
            a = src[i + 0] & 0xff;
            printf("%d\n",a);
            printf("%d\n",k);
            printf("%x\n",a >> 2);
            printf("%x\n",(a & 3) << 4);
            dst[k + 0] = a >> 2;
            // printf("%0x\n",*(unsigned int *)dst);
            dst[k + 1] = (a & 0x3) << 4;
            // printf("%0x\n",*(unsigned int *)dst);
            // dst[k + 2] = '=';
            // dst[k + 3] = '=';
            printf("%0x\n",*(unsigned int *)dst);
            break;

          case 2:
            a = src[i + 0] & 0xff;
            b = src[i + 1] & 0xff;
            dst[k + 0] = table[a >> 2];
            dst[k + 1] = table[((a & 3) << 4) | (b >> 4)];
            dst[k + 2] = table[(b & 0x0f) << 2];
            dst[k + 3] = '=';
            break;
        }
      }

      return dlen;
    }
  }
  using std::tuple;
  using std::make_tuple;
  template<class T>
  using up = std::unique_ptr<T>;
  tuple<up<char>, size_t> encode(const char * src,size_t src_l){
    auto dst_l = base64_encoded_size(src_l);
    up<char> dst(new char(dst_l));
    base64_encode(src,src_l,dst.get(),dst_l);
    return make_tuple(std::move(dst),dst_l);
  }
  tuple<up<char>, size_t> decode(const char * src,size_t src_l){
    auto dst_l = base64_decoded_size(src,src_l);
    up<char> dst(new char(dst_l));
    base64_decode(dst.get(),dst_l,src,src_l);
    return make_tuple(std::move(dst),dst_l);
  }
}
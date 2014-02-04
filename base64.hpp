#include <assert.h>
#include <cstdlib>
#include <memory>
#include <tuple>
#include <vector>
#include <string>

namespace base64{
const int unbase64_table[] =
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
inline int unbase64(size_t x){
    return unbase64_table[x];
}
using std::unique_ptr;
//void encode(const char * src,size_t const src_l,unique_ptr<char []> & dst, size_t & dst_l);
//void decode(const char * src,size_t const src_l,unique_ptr<char []> & dst,size_t & dst_l);
template<typename T1, typename T2>
T2 encode(T1 src,size_t slen, T2 dst){
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
        a = *src & 0xff;++src;
        b = *src & 0xff;++src;
        c = *src & 0xff;++src;

        *dst = table[a >> 2];++dst;
        *dst = table[((a & 3) << 4) | (b >> 4)];++dst;
        *dst = table[((b & 0x0f) << 2) | (c >> 6)];++dst;
        *dst = table[c & 0x3f];++dst;

        i += 3;
        k += 4;
    }

    if (n != slen) {
        switch (slen - n) {
            case 1:
                a = *src & 0xff;
                *dst = table[a >> 2];++dst;
                *dst = table[(a & 0x3) << 4];++dst;
                *dst = '=';++dst;
                *dst = '=';++dst;
            break;

            case 2:
                a = *src & 0xff;++src;
                b = *src & 0xff;
                *dst = table[a >> 2];++dst;
                *dst = table[((a & 3) << 4) | (b >> 4)];++dst;
                *dst = table[(b & 0x0f) << 2];++dst;
                *dst = '=';++dst;
            break;
        }
    }
    return dst;
}
template<typename T1, typename T2>
void decode(T1 src,T1 srcEnd, T2 dst){
    char a,b,c,d;
    while (src < srcEnd) {
        int remaining = srcEnd - src;

        while (unbase64(*src) < 0 && src < srcEnd) ++src, --remaining;
        if (remaining == 0 || *src == '=') break;
        a = unbase64(*src);++src;

        while (unbase64(*src) < 0 && src < srcEnd) ++src, --remaining;
        if (remaining <= 1 || *src == '=') break;
        b = unbase64(*src);++src;

        *dst++ = (a << 2) | ((b & 0x30) >> 4);

        while (unbase64(*src) < 0 && src < srcEnd) ++src, --remaining;
        if (remaining <= 2 || *src == '=') break;
        c = unbase64(*src);++src;

        *dst = ((b & 0x0F) << 4) | ((c & 0x3C) >> 2);++dst;

        while (unbase64(*src) < 0 && src < srcEnd) ++src, remaining--;
        if (remaining <= 3 || *src == '=') break;
        d = unbase64(*src);++src;

        *dst = ((c & 0x03) << 6) | (d & 0x3F);++dst;
    }
}
//template<typename T1,typename T2>
//void encode(T1 src,T1 src_end,T2 dst){
    //while(true){
        //size_t size=3;
        //char buf[size];
        //size_t i;
        //for(i = 0; i < size && src != src_end; ++i,++src){
            //buf[i] = *src;
        //}
        //dst = encode(buf, i, dst);
        //if(src == src_end){
            //break;
        //}
    //}
//}

inline size_t get_base64_encoded_size(size_t size){
    return ((size + 2 - ((size + 2) % 3)) / 3 * 4);
}
inline size_t base64_decoded_size_fast_(size_t size) {
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
template<typename T>
size_t get_base64_decoded_size(T src, size_t size){
    if (size == 0)
        return 0;

    if (src[size - 1] == '=')
        size--;
    if (size > 0 && src[size - 1] == '=')
        size--;

    return base64_decoded_size_fast_(size);
}
template<class T>
using up = std::unique_ptr<T>;
void encode(const char * src,size_t src_l, up<char []> & dst, size_t & dst_l);
void decode(const char * src,size_t src_l, up<char []> & dst, size_t & dst_l);
inline std::string encode(std::string const & src){
    std::string dst;
    dst.resize(get_base64_encoded_size(src.size()));
    encode(src.begin(),src.size(),dst.begin());
    return dst;
}
inline std::string decode(std::string const & src){
    std::string dst;
    dst.resize(get_base64_decoded_size(src.begin(),src.size()));
    decode(src.begin(),src.end(),dst.begin());
    return dst;
}
}


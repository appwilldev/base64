#include <assert.h>
#include <cstdlib>
#include <memory>
#include <tuple>

namespace base64{
  using std::unique_ptr;
  void encode(const char * src,size_t const src_l,unique_ptr<char []> & dst, size_t & dst_l);
  void decode(const char * src,size_t const src_l,unique_ptr<char []> & dst,size_t & dst_l);
}
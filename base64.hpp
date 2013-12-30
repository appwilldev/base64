#include <assert.h>
#include <cstdlib>
#include <memory>
#include <tuple>

namespace base64{
  using std::tuple;
  using std::make_tuple;
  std::tuple<std::unique_ptr<char>, size_t> encode(const char * src,size_t src_l);
  std::tuple<std::unique_ptr<char>, size_t> decode(const char * src,size_t src_l);
}
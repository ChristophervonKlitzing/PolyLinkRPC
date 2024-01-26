#include <gtest/gtest.h>

#include "PolyLinkRPC/result.h"

TEST(result, basic) {
  Result r("bool");

  BytesBuffer b;
  r.serialize_to(b);

  //
}
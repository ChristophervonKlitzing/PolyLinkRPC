#include <gtest/gtest.h>

#include "PolyLinkRPC/bytes.h"

TEST(bytes, bytes_array) {
  BytesBuffer b;
  b.reserve(10);

  b.get_write_range(5, 4);
  ASSERT_EQ(b.size(), 9);

  b.get_write_range(20, 10);
  ASSERT_EQ(b.size(), 30);
}
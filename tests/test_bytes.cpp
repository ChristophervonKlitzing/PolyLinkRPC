#include <gtest/gtest.h>

#include "PolyLinkRPC/bytes.h"

TEST(bytes, bytes_buffer) {
  BytesBuffer b;
  b.reserve(10);

  b.get_write_range(5, 4);
  ASSERT_EQ(b.size(), 9);

  b.get_write_range(20, 10);
  ASSERT_EQ(b.size(), 30);

  b.clear();
  ASSERT_EQ(b.size(), 0);

  b.reserve(10);  // should only allocate memory but not increase size
  ASSERT_EQ(b.size(), 0);
}
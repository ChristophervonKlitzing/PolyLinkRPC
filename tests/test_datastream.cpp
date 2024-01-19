#include <gtest/gtest.h>

#include "PolyLinkRPC/datastream.h"

TEST(datastream, basic) {
  BytesBuffer buffer;
  DataStream stream(buffer);

  stream << static_cast<int16_t>(10);
  ASSERT_EQ(buffer.size(), sizeof(int16_t));

  stream << static_cast<uint64_t>(42);
  ASSERT_EQ(buffer.size(), sizeof(int16_t) + sizeof(uint64_t));

  int16_t v1;
  stream >> v1;
  ASSERT_EQ(v1, 10);

  uint64_t v2;
  stream >> v2;
  ASSERT_EQ(v2, 42);
}
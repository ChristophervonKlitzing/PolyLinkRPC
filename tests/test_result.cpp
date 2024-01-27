#include <gtest/gtest.h>

#include "PolyLinkRPC/result.h"

TEST(result, value) {
  Result res("bool");

  ASSERT_EQ(res.get_value().get_size(), 0);
  ASSERT_STREQ(res.get_value().get_type().c_str(), "bool");

  res.get_value() = Value("int");
  ASSERT_STREQ(res.get_value().get_type().c_str(), "int");

  BytesBuffer buffer;
  res.serialize_to(buffer);
  ASSERT_GE(buffer.size(), 0);

  buffer.clear();
  ASSERT_EQ(buffer.size(), 0);
}

TEST(result, serialization) {
  Result res("bool");
  res.get_value().append_to_value("1234", 4);

  BytesBuffer buffer;
  res.serialize_to(buffer);

  Result res_new;
  res_new.deserialize_from(buffer);

  ASSERT_TRUE(res_new == res);
}
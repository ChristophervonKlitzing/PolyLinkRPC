#include <gtest/gtest.h>

#include "PolyLinkRPC/value.h"

TEST(value, basic) {
  std::string value_type = "float";
  Value v(value_type);

  ASSERT_STREQ(v.get_type().c_str(), value_type.c_str());
  ASSERT_EQ(v.get_size(), 0);

  constexpr int length = 7;
  char data[length];
  for (unsigned int i = 0; i < length; ++i) {
    data[i] = i;
  }
  ASSERT_EQ(sizeof(data), length);
  v.append_to_value(data, sizeof(data));
}
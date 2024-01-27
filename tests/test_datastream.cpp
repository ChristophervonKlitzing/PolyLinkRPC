#include <gtest/gtest.h>

#include "../include/PolyLinkRPC/datastream.hpp"
#include "PolyLinkRPC/endian_utils.h"

EndianFormat get_target_format() {
  /**
   * Returns the endian format which is not used by this system.
   */
  EndianFormat f = get_endian_format() == EndianFormat::BigEndian
                       ? EndianFormat::LittleEndian
                       : EndianFormat::BigEndian;
  return f;
}

template <typename T>  // T should be an integral type
void test_stream_integral_types() {
  BytesBuffer buffer;
  WritableDataStream write_stream(buffer);
  write_stream.set_byte_order(get_target_format());

  write_stream << static_cast<T>(10);
  ASSERT_EQ(buffer.size(), sizeof(T));

  write_stream << static_cast<uint64_t>(42);
  ASSERT_EQ(buffer.size(), sizeof(T) + sizeof(uint64_t));

  ReadableDataStream read_stream(buffer);
  T v1;
  read_stream >> v1;
  ASSERT_EQ(v1, 10);

  uint64_t v2;
  read_stream >> v2;
  ASSERT_EQ(v2, 42);
}

TEST(datastream, integral_types) {
  test_stream_integral_types<uint8_t>();
  test_stream_integral_types<int8_t>();

  test_stream_integral_types<uint16_t>();
  test_stream_integral_types<int16_t>();

  test_stream_integral_types<uint32_t>();
  test_stream_integral_types<int32_t>();

  test_stream_integral_types<uint64_t>();
  test_stream_integral_types<int64_t>();
}

TEST(datastream, string) {
  BytesBuffer buffer;
  WritableDataStream write_stream(buffer);
  write_stream.set_byte_order(get_target_format());

  const std::string str = "Test";
  write_stream << str;

  ASSERT_GE(buffer.size(), str.length());

  ReadableDataStream read_stream(buffer);
  std::string str_new;
  read_stream >> str_new;
  ASSERT_STREQ(str.c_str(), str_new.c_str());
}

TEST(datastream, empty_string) {
  BytesBuffer buffer;
  WritableDataStream write_stream(buffer);
  write_stream.set_byte_order(get_target_format());

  const std::string str = "";
  write_stream << str;

  ReadableDataStream read_stream(buffer);
  std::string str_new;
  read_stream >> str_new;
  ASSERT_EQ(str.size(), str_new.size());
  ASSERT_EQ(str_new.size(), 0);
}

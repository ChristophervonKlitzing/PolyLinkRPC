#include <gtest/gtest.h>

#include <string>

#include "PolyLinkRPC/endian_utils.h"

template <typename T, typename std::enable_if<(sizeof(T) >= 2 &&
                                               std::is_integral<T>::value),
                                              bool>::type = true>
void test_for_type() {
  /**
   * Tests conversion between endian-formats for the given integral type T
   * which must have a size greater or equal to 2.
   */
  EndianFormat f = get_endian_format();

  T native_value = 1;
  T big_endian_value = to_from_big_endian(native_value);

  if (f == EndianFormat::LittleEndian) {
    EXPECT_NE(native_value, big_endian_value)
        << "Converting the memory representation of the int '1' from "
           "little-endian to big-endian should change its value";
  } else if (f == EndianFormat::BigEndian) {
    EXPECT_EQ(native_value, big_endian_value)
        << "Changing the memory representation of the 'int' from big-endian to "
           "big-endian should not change anything";
  } else {
    EXPECT_TRUE(false) << "The system has an unexpected endian format";
  }

  T reconstructed_native_value = to_from_big_endian(big_endian_value);
  EXPECT_EQ(reconstructed_native_value, native_value)
      << "Converting to big-endian and back should result in the same number";
}

TEST(EndianUtils, integral_types) {
  test_for_type<uint16_t>();
  test_for_type<uint32_t>();
  test_for_type<uint64_t>();
}
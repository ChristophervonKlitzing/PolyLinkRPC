#include <gtest/gtest.h>

#include "PolyLinkRPC/versions.h"

TEST(versions, translation) {
  std::vector<std::string> valid_versions = {
      // Edge cases
      "0.0.0", "0.0.63", "0.31.0", "31.0.0", "31.31.63",

      // Some random other versions
      "7.3.1", "0.4.8", "9.7.8"};

  for (const std::string &version_string : valid_versions) {
    uint16_t version_num = versionStringToNumber(version_string);
    std::string reconstructed_version_string =
        versionNumberToString(version_num);
    ASSERT_STREQ(version_string.c_str(), reconstructed_version_string.c_str());
  }
}

TEST(versions, out_of_range) {
  EXPECT_THROW(versionStringToNumber("32.0.0"), std::invalid_argument);
  EXPECT_THROW(versionStringToNumber("0.32.0"), std::invalid_argument);
  EXPECT_THROW(versionStringToNumber("0.0.64"), std::invalid_argument);
  EXPECT_THROW(versionStringToNumber("32.32.64"), std::invalid_argument);
}

TEST(versions, invalid) {
  EXPECT_THROW(versionStringToNumber("."), std::invalid_argument);
  EXPECT_THROW(versionStringToNumber("0a0a0"), std::invalid_argument);
}
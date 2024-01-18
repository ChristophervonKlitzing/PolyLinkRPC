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
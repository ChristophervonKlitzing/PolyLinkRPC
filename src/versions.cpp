#include "PolyLinkRPC/versions.h"

version_number_t versionStringToNumber(const std::string &version) {
  int major, minor, patch;
  char dot;

  std::istringstream iss(version);
  if (!(iss >> major >> dot >> minor >> dot >> patch) || dot != '.' ||
      major > 31 || minor > 31 || patch > 63) {
    throw std::invalid_argument("Invalid version format");
  }

  return (major << 11) | (minor << 6) | patch;
}

std::string versionNumberToString(version_number_t version) {
  int major = (version >> 11) & 0x1F;  // Extracting the major part (5 bits)
  int minor = (version >> 6) & 0x1F;   // Extracting the minor part (5 bits)
  int patch = version & 0x3F;          // Extracting the patch part (6 bits)

  return std::to_string(major) + "." + std::to_string(minor) + "." +
         std::to_string(patch);
}
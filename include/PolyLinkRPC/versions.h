#ifndef VERSIONS_H
#define VERSIONS_H

#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <string>

// version "0.0.0"/ version-number 0 is reserved for unit-testing and debugging.
#define INVALID_VERSION_NUMBER 0
#define VERSION_STRING "0.0.1"  // highest possible version: 31.31.63
#define VERSION_NUMBER versionStringToNumber(VERSION_STRING)

using version_number_t = uint16_t;

version_number_t versionStringToNumber(const std::string &version);
std::string versionNumberToString(version_number_t version);

#endif  // VERSIONS_H
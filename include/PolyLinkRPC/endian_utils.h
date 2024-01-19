#ifndef ENDIAN_UTILS_HPP
#define ENDIAN_UTILS_HPP

#include <cstdint>
#include <limits>
#include <type_traits>

#define ENDIAN_BYTE_SWAP_2(val) \
  ((((val) >> 8) & 0x00FF) | (((val) << 8) & 0xFF00))

// Swap 4 byte, 32 bit values:
#define ENDIAN_BYTE_SWAP_4(val)                                 \
  ((((val) >> 24) & 0x000000FF) | (((val) >> 8) & 0x0000FF00) | \
   (((val) << 8) & 0x00FF0000) | (((val) << 24) & 0xFF000000))

// Swap 8 byte, 64 bit values:
#define ENDIAN_BYTE_SWAP_8(val)                                                \
  ((((val) >> 56) & 0x00000000000000FF) |                                      \
   (((val) >> 40) & 0x000000000000FF00) |                                      \
   (((val) >> 24) & 0x0000000000FF0000) |                                      \
   (((val) >> 8) & 0x00000000FF000000) | (((val) << 8) & 0x000000FF00000000) | \
   (((val) << 24) & 0x0000FF0000000000) |                                      \
   (((val) << 40) & 0x00FF000000000000) |                                      \
   (((val) << 56) & 0xFF00000000000000))

enum class EndianFormat { LittleEndian, BigEndian };

/**
 * Determines the endian-ness of a system at runtime. The 'at runtime' is
 * important if a pre-compiled binary library relies on the endian-ness.
 */
EndianFormat get_endian_format();

/**
 * If the system uses little-endian (determined at runtime):
 * Swaps between the little-endian and big-endian representation of a 8 byte
 integral-type value.

 * If the system uses big-endian:
 * This function does nothing as converting from big-endian to big-endian format
 * does nothing.
 */
template <typename T, typename std::enable_if<(sizeof(T) == 8 &&
                                               std::is_integral<T>::value),
                                              bool>::type = true>
inline T to_from_endian_format(T obj, EndianFormat target_format) {
  if (get_endian_format() == EndianFormat::LittleEndian) {
    return ENDIAN_BYTE_SWAP_8(obj);
  }
  return obj;
}

/**
 * If the system uses little-endian (determined at runtime):
 * Swaps between the little-endian and big-endian representation of a 4 byte
 integral-type value.

 * If the system uses big-endian:
 * This function does nothing as converting from big-endian to big-endian format
 * does nothing.
 */
template <typename T, typename std::enable_if<(sizeof(T) == 4 &&
                                               std::is_integral<T>::value),
                                              bool>::type = true>
inline T to_from_endian_format(T obj, EndianFormat target_format) {
  if (get_endian_format() != target_format) {
    return ENDIAN_BYTE_SWAP_4(obj);
  }
  return obj;
}

/**
 * If the system uses little-endian (determined at runtime):
 * Swaps between the little-endian and big-endian representation of a 2 byte
 integral-type value.

 * If the system uses big-endian:
 * This function does nothing as converting from big-endian to big-endian format
 * does nothing.
 */
template <typename T, typename std::enable_if<(sizeof(T) == 2 &&
                                               std::is_integral<T>::value),
                                              bool>::type = true>
inline T to_from_endian_format(T obj, EndianFormat target_format) {
  if (get_endian_format() != target_format) {
    return ENDIAN_BYTE_SWAP_2(obj);
  }
  return obj;
}

#endif  // ENDIAN_UTILS_HPP

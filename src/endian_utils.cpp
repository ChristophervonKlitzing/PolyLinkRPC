#include "PolyLinkRPC/endian_utils.h"

EndianFormat get_endian_format() {
  uint32_t num = 0x1;
  char *numPtr = reinterpret_cast<char *>(&num);
  return (numPtr[0] == 1) ? EndianFormat::LittleEndian
                          : EndianFormat::BigEndian;
}
#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <cstddef>

#include "bytes.h"

class Communication {
 public:
  virtual bool start() = 0;
  virtual void stop() = 0;
  virtual bool send_packet(const BytesBuffer &buffer) = 0;
  virtual bool recv_packet(BytesBuffer &buffer) = 0;
};

#endif  // COMMUNICATION_H
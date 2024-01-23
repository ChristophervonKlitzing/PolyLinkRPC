#ifndef DATAGRAM_SERIALIZABLE_H
#define DATAGRAM_SERIALIZABLE_H

#include <string>

#include "bytes.h"

class DatagramSerializable {
 private:
  std::string _error_string;

 protected:
  void set_error_string(const std::string &msg);

 public:
  virtual void serialize_to(BytesBuffer &buffer) const = 0;
  virtual bool deserialize_from(BytesBuffer &buffer) = 0;
  const std::string &get_error_string() const;
};

#endif  // DATAGRAM_SERIALIZABLE_H
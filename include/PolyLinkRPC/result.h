#ifndef RESULT_H
#define RESULT_H

#include "datagram.h"
#include "value.h"

/**
 * --- RPC Result datagram ---:
 *
 * | id | Value
 */

class Result : public Datagram {
 private:
  Value _value;

 public:
  Result(const std::string &type);
  Result();
  void serialize_to(BytesBuffer &buffer) const override;
  bool deserialize_from(const BytesBuffer &buffer) override;
};

#endif  // RESULT_H
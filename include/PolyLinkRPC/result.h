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
  Value &get_value();
  void serialize_to(BytesBuffer &buffer) const override;
  bool deserialize_from(const BytesBuffer &buffer) override;

  bool operator==(const Result &other);
};

#endif  // RESULT_H
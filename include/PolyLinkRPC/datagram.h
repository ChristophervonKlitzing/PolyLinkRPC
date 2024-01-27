#ifndef DATAGRAM_H
#define DATAGRAM_H

#include <atomic>
#include <string>

#include "bytes.h"

class Datagram {
 public:
  using id_t = uint64_t;

 private:
  static std::atomic<id_t> _next_id;
  std::string _error_string;
  id_t _id;

 private:
  static id_t create_unique_id();

 protected:
  void set_error_string(const std::string &msg);

 public:
  Datagram();
  void set_id(id_t new_id);
  id_t get_id() const;
  virtual void serialize_to(BytesBuffer &buffer) const = 0;
  virtual bool deserialize_from(const BytesBuffer &buffer) = 0;
  const std::string &get_error_string() const;

  virtual ~Datagram() = default;
};

#endif  // DATAGRAM_H
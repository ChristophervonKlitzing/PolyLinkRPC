#include <gtest/gtest.h>

#include "PolyLinkRPC/datagram.h"

class MockDatagram : public Datagram {
 public:
  void serialize_to(BytesBuffer &buffer) const override{};
  bool deserialize_from(const BytesBuffer &buffer) override { return false; }

  // method for testing is protected -> this makes method public
  void set_error(const std::string &msg) { this->set_error_string(msg); }
};

TEST(datagram, basic) {
  MockDatagram dg;
  ASSERT_STREQ(dg.get_error_string().c_str(), "");

  MockDatagram dg2;
  ASSERT_NE(dg.get_id(), dg2.get_id());  // unique-ness of ids

  std::string error_msg = "abc";
  dg.set_error(error_msg);
  ASSERT_STREQ(dg.get_error_string().c_str(), error_msg.c_str());
}
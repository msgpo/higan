struct Controller : Thread {
  Node::Peripheral node;

  Controller();
  virtual ~Controller();

  auto main() -> void;

  virtual auto read() -> uint4 { return 0x0f; }
  virtual auto write(uint2) -> void {}
};

#include "port.hpp"
#include "gamepad/gamepad.hpp"

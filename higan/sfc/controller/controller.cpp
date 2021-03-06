#include <sfc/sfc.hpp>

namespace higan::SuperFamicom {

#include "port.cpp"
#include "gamepad/gamepad.cpp"
#include "justifier/justifier.cpp"
#include "justifiers/justifiers.cpp"
#include "mouse/mouse.cpp"
#include "ntt-data-keypad/ntt-data-keypad.cpp"
#include "super-multitap/super-multitap.cpp"
#include "super-scope/super-scope.cpp"
#include "twin-tap/twin-tap.cpp"

Controller::Controller() {
  if(!handle()) Thread::create(1, [&] {
    while(true) scheduler.synchronize(), main();
  });
  cpu.peripherals.append(this);
}

Controller::~Controller() {
  Thread::destroy();
}

auto Controller::main() -> void {
  step(1);
  synchronize(cpu);
}

auto Controller::iobit() -> bool {
  if(co_active() == controllerPort1.device.data()) return cpu.pio() & 0x40;
  if(co_active() == controllerPort2.device.data()) return cpu.pio() & 0x80;
  return 1;
}

auto Controller::iobit(bool data) -> void {
  if(co_active() == controllerPort1.device.data()) bus.write(0x4201, cpu.pio() & ~0x40 | data << 6);
  if(co_active() == controllerPort2.device.data()) bus.write(0x4201, cpu.pio() & ~0x80 | data << 7);
}

}

#include <cv/cv.hpp>

namespace higan::ColecoVision {

System system;
Scheduler scheduler;
Cheat cheat;
#include "controls.cpp"
#include "display.cpp"
#include "serialization.cpp"

auto System::run() -> void {
  if(scheduler.enter() == Scheduler::Event::Frame) {
    vdp.refresh();
    controls.poll();
  }
}

auto System::runToSave() -> void {
  scheduler.synchronize(cpu);
  scheduler.synchronize(vdp);
  scheduler.synchronize(psg);
}

auto System::load(Node::Object from) -> void {
  if(node) unload();

  information = {};
  if(interface->name() == "ColecoVision") information.model = Model::ColecoVision;
  if(interface->name() == "ColecoAdam"  ) information.model = Model::ColecoAdam;

  node = Node::System::create(interface->name());
  node->load(from);

  regionNode = Node::String::create("Region", "NTSC");
  regionNode->allowedValues = {"NTSC", "PAL"};
  Node::load(regionNode, from);
  node->append(regionNode);

  scheduler.reset();
  controls.load(node, from);
  display.load(node, from);
  cartridge.load(node, from);
  controllerPort1.load(node, from);
  controllerPort2.load(node, from);
}

auto System::save() -> void {
  if(!node) return;
  cartridge.save();
}

auto System::unload() -> void {
  if(!node) return;
  save();
  cartridge.port = {};
  controllerPort1.port = {};
  controllerPort2.port = {};
  node = {};
}

auto System::power() -> void {
  for(auto& setting : node->find<Node::Setting>()) setting->setLatch();

  if(regionNode->latch() == "NTSC") {
    information.region = Region::NTSC;
    information.colorburst = Constants::Colorburst::NTSC;
  }

  if(regionNode->latch() == "PAL") {
    information.region = Region::PAL;
    information.colorburst = Constants::Colorburst::PAL * 4.0 / 5.0;
  }

  if(auto fp = platform->open(node, "bios.rom", File::Read, File::Required)) {
    fp->read(bios, 0x2000);
  }

  video.reset(interface);
  display.screen = video.createScreen(display.node, display.node->width, display.node->height);
  audio.reset(interface);

  cartridge.power();
  cpu.power();
  vdp.power();
  psg.power();
  scheduler.primary(cpu);

  serializeInit();
}

}

auto System::serialize() -> serializer {
  serializer s(information.serializeSize);

  uint signature = 0x31545342;
  char version[16] = {0};
  char description[512] = {0};
  memory::copy(&version, (const char*)SerializerVersion, SerializerVersion.size());

  s.integer(signature);
  s.array(version);
  s.array(description);

  serializeAll(s);
  return s;
}

auto System::unserialize(serializer& s) -> bool {
  uint signature;
  char version[16] = {0};
  char description[512];

  s.integer(signature);
  s.array(version);
  s.array(description);

  if(signature != 0x31545342) return false;
  if(string{version} != SerializerVersion) return false;

  power();
  serializeAll(s);
  return true;
}

auto System::serialize(serializer& s) -> void {
  s.integer(information.clocksExecuted);
}

auto System::serializeAll(serializer& s) -> void {
  cartridge.serialize(s);
  system.serialize(s);
  cpu.serialize(s);
  ppu.serialize(s);
  apu.serialize(s);
}

auto System::serializeInit() -> void {
  serializer s;

  uint signature = 0;
  char version[16] = {0};
  char description[512] = {0};

  s.integer(signature);
  s.array(version);
  s.array(description);

  serializeAll(s);
  information.serializeSize = s.size();
}

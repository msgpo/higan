namespace higan::Core {

struct Port : Object {
  DeclareClass(Port, "Port")

  Port(string name = {}, string type = {}) : Object(name), type(type) {
    allocate = [&] { return Node::Peripheral::create(); };
  }

  ~Port() {
    disconnect();
  }

  auto connected() -> Node::Peripheral {
    return find<Node::Peripheral>(0);
  }

  auto connect(Node::Peripheral peripheral) -> void {
    disconnect();
    if(attach) attach(peripheral);
  }

  auto disconnect() -> void {
    if(auto peripheral = connected()) {
      if(detach) detach(peripheral);
      remove(peripheral);
    }
  }

  auto serialize(string& output, string depth) -> void override {
    Object::serialize(output, depth);
    output.append(depth, "  type: ", type, "\n");
    output.append(depth, "  hotSwappable: ", hotSwappable, "\n");
  }

  auto unserialize(Markup::Node node) -> void override {
    Object::unserialize(node);
    type = node["type"].string();
    hotSwappable = node["hotSwappable"].boolean();
  }

  string type;
  bool hotSwappable = false;

  function<Node::Peripheral ()> allocate;
  function<void (Node::Peripheral)> attach;
  function<void (Node::Peripheral)> detach;
};

}

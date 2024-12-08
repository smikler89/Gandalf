#include <iostream>
#include <string>
#include <fstream>
#include <iterator>

#include "util/io.h"

#include "config.pb.h"
#include "user_profile.pb.h"

Gandalf::TConfig ReadFromFile(const std::string& fn)
{
  Gandalf::TConfig config;

  std::ifstream f(fn.c_str());
  if (!f.good()) {
    std::cerr << "WARNING: config file not found(" << fn << "), using default one instead" << std::endl;
  } else {
    std::string json(
      (std::istreambuf_iterator<char>(f)),
      std::istreambuf_iterator<char>()
    );
    if (!Gandalf::JsonToProto(json, config)) {
      std::cerr << "WARNING: failed to validate config, using default one instead" << std::endl;
    } else {
      std::cerr << "Config read successfully: " << Gandalf::ProtoToPrettyJson(config) << std::endl;
    }
  }

  return config;
}

int main()
{
  const auto& config = ReadFromFile("./Gandalf.cfg");

  Gandalf::TUserProfile up;
  std::cout << "Hello Proto World!" << std::endl;

  /*std::cout << "Test to JSON / from JSON compression" << std::endl;
  {
    const std::string data = Gandalf::ProtoToJson(config);
    Gandalf::TConfig restored;
    Gandalf::JsonToProto(data, restored);
    std::cout << Gandalf::ProtoToPrettyJson(restored) << std::endl;
  }

  std::cout << "Test to pretty JSON / from JSON compression" << std::endl;
  {
    const std::string data = Gandalf::ProtoToPrettyJson(config);
    Gandalf::TConfig restored;
    Gandalf::JsonToProto(data, restored);
    std::cout << Gandalf::ProtoToPrettyJson(restored) << std::endl;
  }

  std::cout << "Test to binary / from binary compression" << std::endl;
  {
    const std::string data = Gandalf::ProtoToBytes(config);
    Gandalf::TConfig restored;
    Gandalf::BytesToProto(data, restored);
    std::cout << Gandalf::ProtoToPrettyJson(restored) << std::endl;
  }

  std::cout << "Test to ZLib / from ZLib compression" << std::endl;
  {
    const std::string data = Gandalf::ProtoToZLib(config);
    Gandalf::TConfig restored;
    Gandalf::ZLibToProto(data, restored);
    std::cout << Gandalf::ProtoToPrettyJson(restored) << std::endl;
  }*/

  return 0;
}

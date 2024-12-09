#include <iostream>
#include <string>
#include <fstream>
#include <iterator>

#include "util/io.h"
#include "manager/manager_factory.h"

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
    if (!Gandalf::Util::JsonToProto(json, config)) {
      std::cerr << "WARNING: failed to validate config, using default one instead" << std::endl;
    } else {
      std::cerr << "Config read successfully: " << Gandalf::Util::ProtoToPrettyJson(config) << std::endl;
    }
  }

  return config;
}

int main()
{
  const auto& config = ReadFromFile("../Gandalf.cfg");

  auto manager = Gandalf::CreateFileProfileManager("../debug_profiles", Gandalf::PrettyJson);

  Gandalf::TUserProfile up;
  std::cout << "Hello Proto World!" << std::endl;

  const uint32_t now = std::time(0);

  {
    // do nothing, empty profile should not be stored
    Gandalf::RunUserUpdate(
        config,
        *manager,
        "michael", // userId
        {}, // empty events
        now
    );
  }

  {
    // ensure profile don't exist
    manager->DeleteProfile("jane");
    // non-empty list of valid events
    Gandalf::RunUserUpdate(
        config,
        *manager,
        "jane", // userId
        {
          // 1st group - just 1 event
          {Gandalf::EEventType::Product_Purchase, 101 /*itemId*/, now - 1000},
          // 2nd group - 3 events, 2 of them share same itemId but different timestamp
          {Gandalf::EEventType::Product_Visit, 201 /*itemId*/, now - 1000},
          {Gandalf::EEventType::Product_Visit, 202 /*itemId*/, now - 2000},
          {Gandalf::EEventType::Product_Visit, 201 /*itemId*/, now - 3000},
          // 3rd group - 1 event, too old
          {Gandalf::EEventType::ProductCategory_Purchase, 301 /*itemId*/, now - 1000000},
          // 4th group - 10 events, but only 8 could be kept by MAX_ITEMS policy
          {Gandalf::EEventType::ProductBrand_Add, 401 /*itemId*/, now - 1000},
          {Gandalf::EEventType::ProductBrand_Add, 402 /*itemId*/, now - 2000},
          {Gandalf::EEventType::ProductBrand_Add, 403 /*itemId*/, now - 3000},
          {Gandalf::EEventType::ProductBrand_Add, 404 /*itemId*/, now - 4000},
          {Gandalf::EEventType::ProductBrand_Add, 405 /*itemId*/, now - 5000},
          {Gandalf::EEventType::ProductBrand_Add, 406 /*itemId*/, now - 6000},
          {Gandalf::EEventType::ProductBrand_Add, 407 /*itemId*/, now - 7000},
          {Gandalf::EEventType::ProductBrand_Add, 408 /*itemId*/, now - 8000},
          {Gandalf::EEventType::ProductBrand_Add, 409 /*itemId*/, now - 9000},
          {Gandalf::EEventType::ProductBrand_Add, 410 /*itemId*/, now - 10900},
        }, 
        now
    );
  }

  /*std::cout << "Test to JSON / from JSON compression" << std::endl;
  {
    const std::string data = Gandalf::Util::ProtoToJson(config);
    Gandalf::TConfig restored;
    Gandalf::Util::JsonToProto(data, restored);
    std::cout << Gandalf::Util::ProtoToPrettyJson(restored) << std::endl;
  }

  std::cout << "Test to pretty JSON / from JSON compression" << std::endl;
  {
    const std::string data = Gandalf::Util::ProtoToPrettyJson(config);
    Gandalf::TConfig restored;
    Gandalf::Util::JsonToProto(data, restored);
    std::cout << Gandalf::Util::ProtoToPrettyJson(restored) << std::endl;
  }

  std::cout << "Test to binary / from binary compression" << std::endl;
  {
    const std::string data = Gandalf::Util::ProtoToBytes(config);
    Gandalf::TConfig restored;
    Gandalf::Util::BytesToProto(data, restored);
    std::cout << Gandalf::Util::ProtoToPrettyJson(restored) << std::endl;
  }

  std::cout << "Test to ZLib / from ZLib compression" << std::endl;
  {
    const std::string data = Gandalf::Util::ProtoToZLib(config);
    Gandalf::TConfig restored;
    Gandalf::Util::ZLibToProto(data, restored);
    std::cout << Gandalf::Util::ProtoToPrettyJson(restored) << std::endl;
  }*/

  return 0;
}

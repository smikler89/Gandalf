#include "manager_factory.h"

#include "../util/io.h"

#include <fstream>

namespace Gandalf {
    static std::string ReadFileContents(const std::filesystem::path& inFile)
    {
        std::ifstream f(inFile.c_str(), std::ios_base::in | std::ios_base::binary);
        std::string data(
            (std::istreambuf_iterator<char>(f)),
            std::istreambuf_iterator<char>()
        );
        f.close();
        return data;
    }

    static void WriteContentsToFile(const std::string& data, const std::filesystem::path& outFile)
    {
        std::ofstream f(outFile.c_str(), std::ios_base::out | std::ios_base::binary);
        f.write(data.data(), data.size());
        f.flush();
        f.close();
    }

    class TFileBasedProfileManager : public IProfileManager {
    private:
        const std::filesystem::path Directory;
        const ECompressionType CompressionType;
    public:
        TFileBasedProfileManager(const std::string& folderPath, ECompressionType ct)
            : Directory(folderPath)
            , CompressionType(ct)
        {
            if (!std::filesystem::is_directory(Directory)) {
                throw std::runtime_error("TFileBasedProfileManager ERROR: '" + folderPath + "' is not a directory!");
            }
            if (CompressionType != Binary && CompressionType != Json && CompressionType != PrettyJson && CompressionType != ZLib) {
                throw std::runtime_error("TFileBasedProfileManager ERROR: Compression type not supported!");
            }

            std::cerr << "TFileBasedProfileManager created" << std::endl;
        }

        std::shared_ptr<Gandalf::TUserProfile> FetchProfile(const std::string& userId) override
        {
            std::filesystem::path full_path = Directory / userId;
            if (std::filesystem::exists(full_path)) {
                const auto data = ReadFileContents(full_path);
                auto res = std::make_shared<Gandalf::TUserProfile>();
                switch (CompressionType) {
                    case Binary:
                        Gandalf::Util::BytesToProto(data, *res);
                        break;
                    case Json:
                    case PrettyJson:
                        Gandalf::Util::JsonToProto(data, *res);
                        break;
                    case ZLib:
                        Gandalf::Util::ZLibToProto(data, *res);
                        break;
                    default:
                        throw std::runtime_error("TFileBasedProfileManager ERROR: Compression type not supported!");
                        break;
                }
                std::cerr << "TFileBasedProfileManager fetching profile [" << userId << "] from file..." << std::endl;
                return res;
            } else {
                std::cerr << "TFileBasedProfileManager creating new profile [" << userId << "] ..." << std::endl;
                return nullptr;
            }
        }

        void StoreProfile(const std::string& userId, const Gandalf::TUserProfile& profile) override
        {
            std::filesystem::path full_path = Directory / userId;

            std::string data;
            switch (CompressionType) {
                case Binary:
                    data = Gandalf::Util::ProtoToBytes(profile);
                    break;
                case Json:
                    data = Gandalf::Util::ProtoToJson(profile);
                    break;
                case PrettyJson:
                    data = Gandalf::Util::ProtoToPrettyJson(profile);
                    break;
                case ZLib:
                    data = Gandalf::Util::ProtoToZLib(profile);
                    break;
                default:
                    throw std::runtime_error("TFileBasedProfileManager ERROR: Compression type not supported!");
                    break;
            }

            WriteContentsToFile(data, full_path);
            std::cerr << "TFileBasedProfileManager saved profile [" << userId << "] to file..." << std::endl;
        }

        void DeleteProfile(const std::string& userId) override
        {
            std::filesystem::path full_path = Directory / userId;
            if (std::filesystem::exists(full_path)) {
                std::filesystem::remove(full_path);
                std::cerr << "TFileBasedProfileManager deleted [" << userId << "] file..." << std::endl;
            } else {
                std::cerr << "TFileBasedProfileManager ignore deleting [" << userId << "] file..." << std::endl;
            }
        }
    };

    std::shared_ptr<IProfileManager> CreateFileProfileManager(const std::string& folderPath, ECompressionType ct)
    {
        return std::make_shared<TFileBasedProfileManager>(folderPath, ct);
    }
};

#pragma once

#include <google/protobuf/util/json_util.h>

namespace Gandalf {
    namespace Util {
        // JSON

        template<class TProto>
        inline bool JsonToProto(const std::string_view& json, TProto& out)
        {
            const auto& status = google::protobuf::util::JsonStringToMessage(json, &out, {});
            if (!status.ok()) {
                std::cerr << status.message() << std::endl;
                return false;
            }
            return true;
        }

        template<class TProto>
        inline std::string ProtoToJson(const TProto& in)
        {
            std::string res;
            if (!google::protobuf::util::MessageToJsonString(in, &res, {}).ok()) {
                // it's ok
            }
            return res;
        }

        template<class TProto>
        inline std::string ProtoToPrettyJson(const TProto& in)
        {
            google::protobuf::util::JsonPrintOptions opts;
            opts.add_whitespace = true;

            std::string res;
            if (!google::protobuf::util::MessageToJsonString(in, &res, opts).ok()) {
                // it's ok
            }
            return res;
        }


        // Binary format

        template<class TProto>
        inline bool BytesToProto(const std::string& bytes, TProto& out)
        {
            return out.ParseFromString(bytes);
        }

        template<class TProto>
        inline std::string ProtoToBytes(const TProto& in)
        {
            std::string res;
            in.SerializeToString(&res);
            return res;
        }


        // ZLib

        std::string ZLibCompress(const std::string& src);
        std::string ZLibDecompress(const std::string& src);

        template<class TProto>
        inline bool ZLibToProto(const std::string& bytes, TProto& out)
        {
            return out.ParseFromString(ZLibDecompress(bytes));
        }

        template<class TProto>
        inline std::string ProtoToZLib(const TProto& in)
        {
            std::string res;
            in.SerializeToString(&res);
            return ZLibCompress(res);
        }


        // TODO: LZ4
    };
};

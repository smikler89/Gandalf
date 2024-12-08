#include "io.h"

#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/stream.hpp>

#include <sstream>
#include <string>
#include <stdexcept>

namespace Gandalf {
    std::string ZLibCompress(const std::string& src) {
        namespace bio = boost::iostreams;

        std::stringstream compressed;
        std::stringstream original(src);

        bio::filtering_streambuf<bio::input> in;
        in.push(bio::zlib_compressor());
        in.push(original);

        try {
            bio::copy(in, compressed);
        } catch (const boost::iostreams::zlib_error& e) {
            throw std::runtime_error("ZLib compression failed: " + std::string(e.what()));
        }

        return compressed.str();
    }

    std::string ZLibDecompress(const std::string& src) {
        namespace bio = boost::iostreams;

        std::stringstream decompressed;
        std::stringstream compressed(src);

        bio::filtering_streambuf<bio::input> in;
        in.push(bio::zlib_decompressor());
        in.push(compressed);

        try {
            bio::copy(in, decompressed);
        } catch (const boost::iostreams::zlib_error& e) {
            throw std::runtime_error("ZLib decompression failed: " + std::string(e.what()));
        }

        return decompressed.str();
    }
};

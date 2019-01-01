#ifndef JASTERIX_H
#define JASTERIX_H

#include <string>
#include <map>
#include <boost/iostreams/device/mapped_file.hpp>

#include "json.hpp"
#include "frameparser.h"

namespace jASTERIX {

class jASTERIX
{
public:
    jASTERIX(const std::string& filename, const std::string& definition_path, const std::string& framing_str,
             bool debug);
    virtual ~jASTERIX();

    // returns number of frames
    size_t scopeFrames();
    // returns number of decoded records
    size_t decodeFrames();

    void printData();

private:
    std::string filename_;
    std::string definition_path_;
    std::string framing_;
    bool debug_ {false};

    nlohmann::json framing_definition_;
    nlohmann::json record_definition_;
    nlohmann::json asterix_list_definition_;
    std::map<unsigned int, nlohmann::json> asterix_category_definitions_;
    std::unique_ptr<FrameParser> frame_parser_;

    size_t file_size_{0};
    boost::iostreams::mapped_file_source file_;

    nlohmann::json json_data_;
};
}

#endif // JASTERIX_H

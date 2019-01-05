#ifndef COMPOUNDITEMPARSER_H
#define COMPOUNDITEMPARSER_H

#include "itemparser.h"

#include <vector>
#include <memory>

namespace jASTERIX
{
// decodes a field specification/availablity field (ending with extend bit), and list of items
class CompoundItemParser : public ItemParser
{
public:
    CompoundItemParser (const nlohmann::json& item_definition);
    virtual ~CompoundItemParser() {}

    virtual size_t parseItem (const char* data, size_t index, size_t size, size_t current_parsed_bytes,
                              nlohmann::json& target, nlohmann::json& parent, bool debug);
protected:
    std::string field_specification_name_;
    std::unique_ptr<ItemParser> field_specification_;
    std::vector<std::unique_ptr<ItemParser>> items_;
};

}

#endif // COMPOUNDITEMPARSER_H

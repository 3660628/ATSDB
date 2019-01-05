#include "fixedbitfielditemparser.h"
#include "fixedbitsitemparser.h"

using namespace std;
using namespace nlohmann;

namespace jASTERIX
{

FixedBitFieldItemParser::FixedBitFieldItemParser (const nlohmann::json& item_definition)
 : ItemParser (item_definition)
{
    assert (type_ == "fixed_bitfield");

    optional_ = item_definition.find("optional") != item_definition.end() && item_definition.at("optional") == true;

    if (optional_)
    {
        if (item_definition.find("optional_variable_name") == item_definition.end())
            throw runtime_error ("parsing fixed bitfield item '"+name_+"' optional but no variable given");

        optional_variable_name_ = item_definition.at("optional_variable_name");

        if (item_definition.find("optional_variable_value") == item_definition.end())
            throw runtime_error ("parsing fixed bitfield item '"+name_+"' optional but no variable value given");

        optional_variable_value_ = item_definition.at("optional_variable_value");
    }

    if (item_definition.find("length") == item_definition.end())
        throw runtime_error ("parsing fixed bitfield item '"+name_+"' without length");

    length_ = item_definition.at("length");

    if (length_ > 8)
        throw runtime_error ("parsing fixed bitfield item '"+name_+"' with too big length");

    if (item_definition.find("items") == item_definition.end())
        throw runtime_error ("parsing fixed bitfield item '"+name_+"' without sub-items");

    const json& items = item_definition.at("items");

    if (!items.is_array())
        throw runtime_error ("parsing fixed bitfield item '"+name_+"' sub-items specification is not an array");

    std::string item_name;
    FixedBitsItemParser* item {nullptr};

    for (const json& data_item_it : items)
    {
        item_name = data_item_it.at("name");
        item = new FixedBitsItemParser(data_item_it, length_);
        assert (item);
        items_.push_back(std::unique_ptr<ItemParser>{item});
    }
}

size_t FixedBitFieldItemParser::parseItem (const char* data, size_t index, size_t size, size_t current_parsed_bytes,
                              nlohmann::json& target, nlohmann::json& parent, bool debug)
{
    if (debug)
        loginf << "parsing fixed bitfield item '" << name_ << "'";

    if (optional_ && !variableHasValue(parent, optional_variable_name_, optional_variable_value_))
    {
        if (debug) //  in '" << parent.dump(4) << "'"
            loginf << "parsing fixed bitfield item '" << name_ << "' skipped since variable '"
                   << optional_variable_name_ << "' not set";

        return 0; // no parse
    }

    for (auto& sub_item_it : items_)
    {
        if (debug)
            loginf << "parsing fixed bitfield item '" << name_ << "' item '" << sub_item_it->name() << "'";

        sub_item_it->parseItem(data, index, size, current_parsed_bytes, target[sub_item_it->name()], parent, debug);
    }

    return length_;
}

}

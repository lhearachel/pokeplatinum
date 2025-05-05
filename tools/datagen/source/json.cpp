#include "json.h"

#include <rapidjson/document.h>

unsigned int json::uint(const rapidjson::Value &val)
{
    // TODO: Figure out how to get the key into this exception type...
    if (!val.IsUint()) {
        throw new JsonIncorrectType("expected number type");
    }
    return val.GetUint();
}

std::string json::asstring(const rapidjson::Value &val)
{
    if (!val.IsString()) {
        throw new JsonIncorrectType("expected string type");
    }
    return val.GetString();
}

bool json::asbool(const rapidjson::Value &val)
{
    if (!val.IsBool()) {
        throw new JsonIncorrectType("expected boolean type");
    }
    return val.GetBool();
}

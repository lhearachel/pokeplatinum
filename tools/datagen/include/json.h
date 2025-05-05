#pragma once

#include <exception>
#include <filesystem>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <rapidjson/error/error.h>
#include <string>

#include "utils.h"

namespace fs = std::filesystem;

struct Slice {
    long begin;
    long length;
};

struct JsonParseError : public std::exception {
    std::string m_msg;
    std::string m_inp;
    long m_begin;
    long m_length;
    long m_linenum;
    long m_colnum;

    JsonParseError(const rapidjson::ParseResult &err, const std::string indata, const fs::path &inp)
    {
        std::vector<Slice> linecoords { Slice { 0, 0 } };
        for (int i = 0; i < indata.length(); i++) {
            if (indata.at(i) == '\n') {
                linecoords.back().length = i - linecoords.back().begin;
                linecoords.emplace_back(Slice { i + 1, 0 });
            }
        }
        linecoords.back().length = indata.length() - linecoords.back().begin;

        auto line = std::find_if(linecoords.begin(), linecoords.end(), [&err](Slice slice) {
            return slice.length + slice.begin >= err.Offset();
        });
        auto lineidx = std::distance(linecoords.begin(), line);

        this->m_msg = rapidjson::GetParseError_En(err.Code());
        this->m_inp = fs::relative(inp).generic_string();
        this->m_begin = line->begin;
        this->m_length = line->length;
        this->m_linenum = lineidx + 1;
        this->m_colnum = err.Offset() - line->begin + 1;
    }

    const char *what() const noexcept override
    {
        return this->m_msg.c_str();
    }
};

struct JsonKeyNotFound : public BasicError {
    JsonKeyNotFound(const char *msg)
        : BasicError(msg)
    {
    }
};

struct JsonIncorrectType : public BasicError {
    JsonIncorrectType(const char *msg)
        : BasicError(msg)
    {
    }
};

struct Json {
    rapidjson::Document m_doc;

    Json(const fs::path &inp)
    {
        const std::string indata = readwhole(inp);
        const rapidjson::ParseResult ok = this->m_doc.Parse(indata.c_str(), indata.length());
        if (!ok) {
            throw JsonParseError(ok, indata, inp);
        }
    }

    rapidjson::Value &operator[](const char *key)
    {
        auto memb = this->m_doc.FindMember(key);
        if (memb == this->m_doc.MemberEnd()) {
            throw JsonKeyNotFound(key);
        }
        return memb->value;
    }
};

namespace json {

unsigned int uint(const rapidjson::Value &val);

std::string asstring(const rapidjson::Value &val);

bool asbool(const rapidjson::Value &val);

};

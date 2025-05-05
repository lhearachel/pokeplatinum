#include "utils.h"

#include <filesystem>
#include <fstream>
#include <regex>
#include <string>
#include <vector>

const std::vector<std::string> tokenize(const std::string &s, const std::regex &r) noexcept
{
    return std::vector<std::string>(
        std::sregex_token_iterator(s.begin(), s.end(), r, -1),
        std::sregex_token_iterator());
}

const std::vector<std::string> splitenv(const char *var, const std::regex &r)
{
    const char *val_p = std::getenv(var);
    if (val_p == nullptr) {
        throw EnvVarNotFound(var);
    }
    return tokenize(val_p, std::regex(";"));
}

const std::string readwhole(const fs::path &p)
{
    std::ifstream ifs(p, std::ios::in);
    if (!ifs.is_open()) {
        throw FileNotFound(p.c_str());
    }

    ifs.seekg(0, std::ios::end);
    std::size_t fsize = ifs.tellg();
    std::string whole(fsize, ' ');
    ifs.seekg(0);
    ifs.read(&whole[0], fsize);

    return whole;
}

const std::vector<std::string> readlines(const fs::path &p)
{
    std::ifstream ifs(p, std::ios::in);
    if (!ifs.is_open()) {
        throw FileNotFound(p.c_str());
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(ifs, line)) {
        lines.emplace_back(line);
    }

    ifs.close();
    return lines;
}

#pragma once

#include <exception>
#include <filesystem>
#include <regex>
#include <string>
#include <vector>

namespace fs = std::filesystem;

struct BasicError : public std::exception {
    std::string m_msg;

    BasicError(const char *msg)
        : m_msg(msg)
    {
    }

    const char *what() const noexcept override
    {
        return this->m_msg.c_str();
    }
};

struct EnvVarNotFound : public BasicError {
    EnvVarNotFound(const char *msg)
        : BasicError(msg)
    {
    }
};

struct FileNotFound : public BasicError {
    FileNotFound(const char *msg)
        : BasicError(msg)
    {
    }
};

/*
 * Tokenize an input string according to the given regex.
 */
const std::vector<std::string> tokenize(const std::string &s, const std::regex &r = std::regex(" ")) noexcept;

/*
 * Tokenize an environment variable value according to the given regex.
 *
 * Throws EnvVarNotFound when the environment variable does not exist.
 */
const std::vector<std::string> splitenv(const char *var, const std::regex &r = std::regex(" "));

/*
 * Read an input file into a string.
 *
 * Throws FileNotFound when the file does not exist or is malformed.
 */
const std::string readwhole(const fs::path &p);

/*
 * Read an input file's lines.
 *
 * Throws FileNotFound when the file does not exist or is malformed.
 */
const std::vector<std::string> readlines(const fs::path &p);

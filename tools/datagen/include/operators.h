#pragma once

#include <vector>

template <typename T>
std::vector<T> operator+(const std::vector<T> &a, const std::vector<T> &b)
{
    std::vector<T> ab;
    ab.reserve(a.size() + b.size());
    ab.insert(ab.end(), a.begin(), a.end());
    ab.insert(ab.end(), b.begin(), b.end());
    return ab;
}

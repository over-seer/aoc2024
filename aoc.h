#ifndef MY_AOC_HEADER_H
#define MY_AOC_HEADER_H

#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <deque>
#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <locale>
#include <map>
#include <numeric>
#include <optional>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace aoc {
template <class T>
std::set<T> intersect(const std::set<T> &s1, const std::set<T> &s2) {
  std::set<T> result;
  set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(),
                   inserter(result, result.begin()));
  return result;
}

inline std::vector<std::string> split(const std::string &line, char c) {
  std::vector<std::string> result;
  std::stringstream ss(line);
  std::string s;
  while (std::getline(ss, s, c)) {
    result.push_back(s);
  }
  return result;
}

template <size_t N> std::array<std::string, N> splitn(const std::string &line, char c = ' ') {
  std::array<std::string, N> result;
  std::stringstream ss(line);
  for (size_t i = 0; i < N; i++)
    std::getline(ss, result[i], c);
    //ss >> result[i];
  return result;
}

inline void ltrim_in_place(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
          }));
}

// trim from end (in place)
inline void rtrim_in_place(std::string &s) {
  auto it = std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
              return !std::isspace(ch);
            }).base();
  s.erase(it, s.end());
}

// trim from both ends (in place)
inline void trim_in_place(std::string &s) {
  rtrim_in_place(s);
  ltrim_in_place(s);
}

// trim from start (copying)
inline std::string ltrim(std::string s) {
  ltrim_in_place(s);
  return s;
}

// trim from end (copying)
inline std::string rtrim(std::string s) {
  rtrim_in_place(s);
  return s;
}

// trim from both ends (copying)
inline std::string trim(std::string s) {
  trim_in_place(s);
  return s;
}

inline auto read_file(std::string s) {
  std::vector<std::string> lines;
  std::string line;
  std::ifstream ip(s);
  while (std::getline(ip, line))
    lines.push_back(line);
  return lines;
}

inline auto read_paragraph_ip(std::string s) {
  std::vector<std::vector<std::string>> result;
  std::vector<std::string> para;
  auto lines = read_file(s);
  std::set<size_t> breaks = {0, lines.size()};
  for (auto line : lines) {
    if (!line.empty()) {
      para.push_back(line);
    } else {
      if (!para.empty())
        result.push_back(para);
      para.clear();
    }
  }
  if (!para.empty())
    result.push_back(para);
  return result;
}

template <class T>
void merge(std::set<T> &a, const std::set<T> &b) {
  for (auto &k : b) {
    a.insert(k);
  }
}

// add padding round a vector of strings
std::vector<std::string> pad(std::vector<std::string> v, char c = '0', size_t n = 1) {
  const size_t nx = v.at(0).size();
  const std::string lr = std::string(n, c);
  for (auto &s : v) {
    s = std::format("{}{}{}", lr, s, lr);
    assert(s.size() == nx + 2 * n);
  }
  std::vector<std::string> tb(n, std::string(nx + 2 * n, c));
  v.insert(v.begin(), tb.begin(), tb.end());
  v.insert(v.end(), tb.begin(), tb.end());
  return v;
}

} // namespace aoc

namespace std {
template <typename T, std::size_t N> struct hash<std::array<T, N>> {
  std::size_t operator()(const std::array<T, N> &arr) const {
    std::hash<T> hasher;
    std::size_t hash_value = 0;

    for (const auto &element : arr) {
      hash_value ^=
          hasher(element) + 0x9e3779b9 + (hash_value << 6) + (hash_value >> 2);
    }

    return hash_value;
  }
};
} // namespace std

#endif

#include "../aoc.h"
#include <functional>
#include <print>
#include <ranges>
#include <stdexcept>

using std::ranges::count_if;
using std::runtime_error;
using std::string, std::vector;
using std::println;

auto parse(const string &fn) {
  const auto ip = aoc::read_file(fn);
  vector<vector<int>> res;
  res.reserve(ip.size());
  for (auto &line : ip) {
    auto words = aoc::split(line, ' ');
    vector<int> v;
    v.reserve(words.size());
    for (auto &word : words) {
      int i = stoi(word);
      v.push_back(i);
    }
    res.push_back(v);
  }
  return res;
}

bool safe(const vector<int> &v) {
  if (v.empty())
    throw runtime_error("empty report!");
  if (v.size() == 1)
    return true;

  const int d1 = v[1] - v[0];
  if (abs(d1) > 3 || abs(d1) < 1)
    return false;

  const bool is_increasing = d1 > 0 ? true : false;

  int last = v[1];
  for (size_t i = 2; i < v.size(); i++) {
    const int d = is_increasing ? v[i] - last : last - v[i];
    if (d > 3 || d < 1)
      return false;
    last = v[i];
  }
  return true;
}

auto rmel(vector<int> v, size_t i) {
  v.erase(v.begin() + i);
  return v;
}

void part1(const string &fn) {
  const auto ip = parse(fn);
  const auto ans1 = count_if(ip, safe);
  println("part1 ans = {}", ans1);

  const auto ans2 = count_if(ip, [](const vector<int> &v) {
    if (safe(v))
      return true;
    for (size_t i = 0; i < v.size(); i++) {
      auto v2 = rmel(v, i);
      if (safe(v2))
        return true;
    }
    return false;
  });

  println("part2 ans = {}", ans2);
}

int main() {
  part1("test_input");
  part1("input");
}

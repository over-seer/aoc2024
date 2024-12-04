#include "../aoc.h"
#include <algorithm>
#include <array>
#include <print>
#include <ranges>
#include <string>
#include <vector>

using std::accumulate, std::ranges::count, std::sort;
using std::array, std::string, std::vector;
using std::println;

auto parse1(const string &fn) {
  array<vector<int64_t>, 2> op;
  const auto ip = aoc::read_file(fn);
  for (auto &line : ip) {
    auto [s0, s1] = aoc::splitn<2>(line);
    op[0].push_back(stoi(s0));
    op[1].push_back(stoi(s1));
  }
  return op;
}

void part1(const string &fn) {
  auto [l, r] = parse1(fn);
  sort(l.begin(), l.end());
  sort(r.begin(), r.end());
  int64_t ans = 0;
  for (size_t i = 0; i < l.size(); i++) {
    auto diff = std::abs(l.at(i) - r.at(i));
    ans += diff;
  }
  println("part 1 ans = {}", ans);
}

void part2(const string &fn) {
  const auto [l, r] = parse1(fn);
  int64_t ans = 0;
  for (auto x : l)
    ans += count(r, x) * x;
  println("part 2 ans = {}", ans);
}

int main() {
  part1("test_input");
  part1("input");
  part2("test_input");
  part2("input");
}
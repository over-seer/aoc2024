#include "aoc.h"
#include <algorithm>
#include <array>
#include <print>
#include <ranges>
#include <string>
#include <vector>
#include <functional>

using std::accumulate, std::ranges::count, std::ranges::sort, std::transform_reduce;
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
  sort(l);
  sort(r);
  int64_t ans = 0;
  for (size_t i = 0; i < l.size(); i++) {
    auto diff = std::abs(l.at(i) - r.at(i));
    ans += diff;
  }
  println("part 1 ans = {}", ans);
}

void part2(const string &fn) {
  const auto [l, r] = parse1(fn);
  const int64_t ans = transform_reduce(l.begin(), l.end(), 0, std::plus<>(),
                                       [&](auto x) { return count(r, x) * x; });
  println("part 2 ans = {}", ans);
}

int main() {
  part1("test_input");
  part1("input");
  part2("test_input");
  part2("input");
}
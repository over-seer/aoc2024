#include "aoc.h"
#include <bitset>
#include <print>
#include <ranges>
#include <string>
#include <vector>

using std::println;
using std::string, std::vector;
using Secret = std::bitset<24>;

Secret evolve(Secret x) {
  x = x ^ (x << 6);
  x = x ^ (x >> 5);
  return x ^ (x << 11);
}

void part1(const string &fn) {
  const auto ip =
      aoc::read_file(fn) |
      std::views::transform([](const string &s) { return std::stoull(s); }) |
      std::ranges::to<vector>();
  auto gen = [](long long unsigned i) {
    Secret x{i};
    for (int ix = 0; ix < 2000; ix++) {
      x = evolve(x);
    }
    println("{}", x.to_ulong());
    return x.to_ullong();
  };
  const auto ans =
      std::ranges::fold_left(ip | std::views::transform(gen), 0, std::plus());
  println("{} day 22 part 1 ans = {}", fn, ans);
}

int main() {
  part1("test_input");
  part1("input");
}

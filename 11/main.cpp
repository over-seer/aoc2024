#include "aoc.h"
#include <algorithm>
#include <functional>
#include <map>
#include <optional>
#include <print>
#include <ranges>
#include <string>
#include <vector>

using std::map, std::optional, std::string, std::vector;
using std::print, std::println, std::stoul;

struct NewStones {
  size_t stone1;
  optional<size_t> stone2;
};

size_t ndigits(size_t i) {
  size_t x10 = 10;
  for (size_t ix = 1; ix < 18; ix++, x10 *= 10)
    if (i < x10) {
      return ix;
    }
  throw std::runtime_error("too big!");
}

size_t pow10(size_t n) {
  size_t tenn = 1;
  for (size_t i = 0; i < n; i++)
    tenn *= 10;
  return tenn;
}

NewStones f(size_t istone) {
  if (istone == 0) {
    return NewStones{1, std::nullopt};
  } else if (size_t nd = ndigits(istone); nd % 2 == 0) {
    const size_t r = istone % (pow10(nd / 2));
    const size_t l = istone / (pow10(nd / 2));
    return NewStones{l, r};
  } else {
    return NewStones{istone * 2024, std::nullopt};
  }
}

void increment_counter(auto &counter, const auto &key, size_t n) {
  if (!counter.contains(key)) {
    counter[key] = n;
  } else {
    counter[key] += n;
  }
}

auto blink(map<size_t, size_t> &stones) {
  map<size_t, size_t> new_stones;
  for (auto [istone, nstones] : stones) {
    auto [stone1, another] = f(istone);
    increment_counter(new_stones, stone1, nstones);
    if (another.has_value()) {
      increment_counter(new_stones, *another, nstones);
    }
  }
  return new_stones;
}

map<size_t, size_t> make_counter(const vector<string> &ip) {
  map<size_t, size_t> counter;
  for (const string &i : ip) {
    increment_counter(counter, stoul(i), 1);
  }
  return counter;
}

void part1(const string &ip) {
  auto counter = make_counter(aoc::split(ip, ' '));
  for (size_t i = 0; i < 25; i++) {
    counter = blink(counter);
  }
  const size_t ans = std::ranges::fold_left(counter | std::views::values, 0, std::plus());
  println("part 1 ans = {}", ans);
}

void part2(const string &ip) {
  auto counter = make_counter(aoc::split(ip, ' '));
  for (size_t i = 0; i < 75; i++) {
    counter = blink(counter);
  }
  const size_t ans = std::ranges::fold_left(counter | std::views::values, 0, std::plus());
  println("part 2 ans = {}", ans);
}

int main() {
  part1("125 17");
  part1(aoc::read_file("input").at(0));
  part2(aoc::read_file("input").at(0));
}
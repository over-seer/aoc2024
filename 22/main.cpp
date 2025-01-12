#include "aoc.h"
#include <bitset>
#include <print>
#include <ranges>
#include <string>
#include <vector>
//#include <execution>

using namespace std;
using Secret = bitset<24>;

Secret evolve(Secret x) {
  x = x ^ (x << 6);
  x = x ^ (x >> 5);
  return x ^ (x << 11);
}

auto parse(const string &fn) {
  return aoc::read_file(fn) |
         views::transform([](const string &s) { return stoull(s); }) |
         ranges::to<vector>();
}

auto gen(long long unsigned i) {
  Secret x{i};
  for (int ix = 0; ix < 2000; ix++) {
    x = evolve(x);
  }
  return x.to_ullong();
}

void part1(const string &fn) {
  const auto ip = parse(fn);
  const auto ans = ranges::fold_left(ip | views::transform(gen), 0, plus());
  println("{} day 22 part 1 ans = {}", fn, ans);
}

array<int, 2001> get_prices(long long unsigned i) {
  array<int, 2001> prices;
  prices[0] = i % 10;
  Secret x{i};
  for (auto &i : prices | views::drop(1)) {
    const auto y = evolve(x);
    i = static_cast<int>(y.to_ulong() % 10);
    assert(i >= 0 && i < 10);
    x = y;
  }
  return prices;
}

int get_total_for_seq(const vector<map<array<int, 4>, int>> &grps,
                      const array<int, 4> &seq) {
  auto f = [&](const auto &g) {
    if (g.contains(seq)) {
      return g.at(seq);
    } else
      return 0;
  };
  //return transform_reduce(std::execution::par_unseq, grps.begin(), grps.end(), 0, plus(), f);
  return transform_reduce(grps.begin(), grps.end(), 0, plus(), f);
  // return ranges::fold_left(grps | views::transform(f));
}

auto get_seqs(const array<int, 2001> &prices) {
  map<array<int, 4>, int> result;
  for (auto [a, b, c, d, e] : prices | views::adjacent<5> | views::reverse) {
    result[{b - a, c - b, d - c, e - d}] = e;
  }
  return result;
};

void part2(const string &fn) {
  const auto nos = parse(fn);
  vector<array<int, 2001>> prices(nos.size());
  for (auto &&[ix, d] : prices | views::enumerate) {
    d = get_prices(nos[ix]);
  }

  const auto grps = prices | views::transform(get_seqs) | ranges::to<vector>();
  const auto changes = grps | views::join | views::keys | ranges::to<set>();
  int ans = 0;
  for (auto seq : changes) {
    const int bananas = get_total_for_seq(grps, seq);
    ans = max(bananas, ans);
  }
  println("debug {}", get_total_for_seq(grps, {-2, 1, -1, 3}));
  println("{}", ans);
}

int main() {
  try {
    part1("test_input");
    part1("input");
    part2("test_input2");
    part2("input");
  } catch (const exception &e) {
    println("{}", e.what());
  }
}

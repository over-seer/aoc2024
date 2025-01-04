#include "aoc.h"
#include <algorithm>
#include <array>
#include <format>
#include <functional>
#include <map>
#include <print>
#include <ranges>
#include <set>
#include <string>
#include <vector>

using namespace std;

using Vec = array<int, 2>;

enum IJ { ROW, COL };

constexpr array nkeys = {string_view{"789"}, string_view{"456"},
                         string_view{"123"}, string_view{"X0A"}};

constexpr array dkeys = {string_view{"X^A"}, string_view{"<v>"}};

vector<string> parse(const string &fn) {
  return aoc::read_file(fn) |
         views::transform([](const string &s) { return format("A{}", s); }) |
         ranges::to<vector>();
}

constexpr map<char, Vec> keypos(const auto keys) {
  map<char, Vec> result;
  for (auto [irow, s] : keys | views::enumerate) {
    for (auto [icol, c] : s | views::enumerate) {
      if (c != 'X')
        result[c] = Vec{static_cast<int>(irow), static_cast<int>(icol)};
    }
  }
  return result;
}

auto paths_recurs(const auto &keys, Vec curr, const Vec targ, string path,
                  vector<string> &paths) {
  //println("debug {}",keys[curr[ROW]][curr[COL]]);
  if(keys[curr[ROW]][curr[COL]] == 'X') return;
  if (curr == targ) {
    paths.push_back(format("{}A", path));
    return;
  }

  const auto [ir0, ic0] = curr;
  const auto [ir1, ic1] = targ;
  if (ir0 < ir1) {
    Vec next{ir0 + 1, ic0};
    if (keys.at(next[ROW]).at(next[COL] != 'X')) {
      paths_recurs(keys, next, targ, format("{}v", path), paths);
    }
  }
  if (ir0 > ir1) {
    Vec next{ir0 - 1, ic0};
    if (keys.at(next[ROW]).at(next[COL] != 'X')) {
      paths_recurs(keys, next, targ, format("{}^", path), paths);
    }
  }
  if (ic0 < ic1) {
    Vec next{ir0, ic0 + 1};
    if (keys.at(next[ROW]).at(next[COL] != 'X')) {
      paths_recurs(keys, next, targ, format("{}>", path), paths);
    }
  }
  if (ic0 > ic1) {
    Vec next{ir0, ic0 - 1};
    if (keys.at(next[ROW]).at(next[COL] != 'X')) {
      paths_recurs(keys, next, targ, format("{}<", path), paths);
    }
  }
}

auto all_paths(const auto &keys) {
  map<array<char, 2>, vector<string>> result;
  const auto ks = keys | views::join;
  const auto pos = keypos(keys);
  for (auto [c0, c1] : views::cartesian_product(ks, ks)) {
    if (c0 != 'X' && c1 != 'X') {
      vector<string> paths;
      paths_recurs(keys, pos.at(c0), pos.at(c1), string{""}, paths);
      result[{c0, c1}] = paths;
    }
  }
  return result;
};

size_t dirseq(auto &cache, const auto &paths, const auto &dirpaths, char c0,
              char c1, int n) {
  if(cache.contains({c0,c1,n})) return cache.at({c0,c1,n});
  size_t result{};
  if (n == 0) {
    result = paths.at({c0, c1}).at(0).size();
  } else {
    auto get_suff_len = [&](const string &s) {
      size_t suff_len = 0;
      for (auto [c2, c3] : (string{"A"} + s) | views::adjacent<2>) {
        suff_len += dirseq(cache, dirpaths, dirpaths, c2, c3, n - 1);
      }
      return suff_len;
    };
    const auto suffixes = paths.at({c0, c1}) | views::transform(get_suff_len);
    result = ranges::min(suffixes);
  }
  cache[{c0,c1,n}] = result;
  return result;
}

size_t seq(const auto &numpaths, const auto &dirpaths, const string &no, int nrobots) {
  size_t result = 0;
  map<tuple<char,char,int>,size_t> cache;
  for (auto [c0, c1] : no | views::adjacent<2>) {
    result += dirseq(cache, numpaths, dirpaths, c0, c1, nrobots);
  }
  return result;
}

void print(auto &keypaths) {
  for (auto &[cc, vs] : keypaths)
    for (auto &s : vs)
      println("keys {} >>> {} {}", cc[0], cc[1], s);
  println();
}

void part1(const vector<string> &nos) {
  const auto num_key_paths = all_paths(nkeys);
  const auto dir_key_paths = all_paths(dkeys);
  //print(num_key_paths);
  //print(dir_key_paths);
  const auto complexity =
      nos | views::transform([&](string no) {
        return seq(num_key_paths, dir_key_paths, no, 2) *
               stoi(no.substr(1, 3));
      });
  const auto ans = ranges::fold_left(complexity, 0, plus());
  println("part 1 ans = {}", ans);
}

void part2(const vector<string> &nos) {
  const auto num_key_paths = all_paths(nkeys);
  const auto dir_key_paths = all_paths(dkeys);
  const auto complexity =
      nos | views::transform([&](string no) {
        return seq(num_key_paths, dir_key_paths, no, 25) *
               stoi(no.substr(1, 3));
      });
  const auto ans = ranges::fold_left(complexity, 0, plus());
  println("part 2 ans = {}", ans);
}

int main() {
  for (auto r : nkeys)
    println("{}", r.data());

  try {
    part1(parse("test_input"));
    part1(parse("input"));
    part2(parse("input"));
  } catch (const exception &e) {
    println("{}", e.what());
  }
}
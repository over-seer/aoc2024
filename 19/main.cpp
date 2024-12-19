#include "aoc.h"
#include <algorithm>
#include <print>
#include <ranges>
#include <set>
#include <utility>
#include <vector>
using namespace std;

struct Ip {
  set<string> stripes;
  vector<string> patterns;
};

auto parse(const string &fn) -> Ip {
  auto ip = aoc::read_file(fn);
  auto stripes =
      (string(" ") + ip.at(0)) | views::split(',') |
      views::transform(views::drop(1)) |
      views::transform([](auto s) { return string(string_view(s)); }) |
      ranges::to<set>();
  auto patterns = ip | views::drop(2) | ranges::to<vector>();
  return Ip{stripes, patterns};
}

bool is_match(const auto &stripes, const string_view str) {
  if (str.size() == 0)
    return true;
  for (size_t n = 1; n <= str.size(); n++) {
    auto sub = str | views::take(n);
    if (stripes.contains(string(sub)) &&
        is_match(stripes, string_view(str | views::drop(n))))
      return true;
  }
  return false;
}

void part1(const string &fn) {
  const auto [stripes, patterns] = parse(fn);
  const auto ans =
      ranges::count_if(patterns, [&](auto s) { return is_match(stripes, s); });
  std::println("part 1 ans = {}", ans);
}

int64_t num_match(const auto &lengths, int64_t ix, auto &cache) {

  if (ix == lengths.size()) {
    return 1;
  } else if(ix > lengths.size()) {
    return 0;
  }
  if (cache.contains(ix)) {
    return cache.at(ix);
  }
  int64_t count = 0;
  for (int64_t len : lengths.at(ix)) {
    count += num_match(lengths, ix + len, cache);
  }
  cache[ix] = count;
  return count;
}

void part2(const string &fn) {
  const auto [stripes, patterns] = parse(fn);
  int64_t ans = 0;
  for (const string pattern : patterns) {
    vector<set<int64_t>> lengths(pattern.size());
    for (int64_t i = 0; i < pattern.size(); i++) {
      string_view sv = string_view(pattern | views::drop(i));
      for (const auto &s : stripes) {
        if (sv.starts_with(s)) {
          int64_t len = static_cast<int64_t>(s.size());
          lengths[i].insert(len);
        }
      }
    }
    map<int64_t,int64_t> cache;
    ans += num_match(lengths,0,cache);
  }
  std::println("part 2 ans = {}", ans);
}

int main() {
  part1("test_input");
  part1("input");
  part2("test_input");
  part2("input");
}
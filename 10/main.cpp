#include "aoc.h"
#include <print>
#include <ranges>

using std::array, std::set, std::string, std::vector;
using std::print, std::println;

using std::views::enumerate;

int pathfind(const vector<string> &chart, const size_t ir, const size_t ic,
             set<array<size_t, 2>> &visited) {
  constexpr array dirs = {array{0, 1}, array{0, -1}, array{1, 0}, array{-1, 0}};
  if(visited.contains({ir,ic})) return 0;
  visited.insert({ir,ic});
  if (chart.at(ir).at(ic) == '9') {
    return 1;
  }
  int ans = 0;
  for (auto [dr, dc] : dirs) {
    const size_t jr = ir + dr;
    const size_t jc = ic + dc;
    // println("{},{}",jr,jc);
    if (chart.at(jr).at(jc) - chart.at(ir).at(ic) == 1) {
      ans += pathfind(chart, jr, jc, visited);
    }
  }
  return ans;
}

int pathfind(const vector<string> &chart, const size_t ir, const size_t ic) {
  constexpr array dirs = {array{0, 1}, array{0, -1}, array{1, 0}, array{-1, 0}};
  //if(visited.contains({ir,ic})) return 0;
  //visited.insert({ir,ic});
  if (chart.at(ir).at(ic) == '9') {
    return 1;
  }
  int ans = 0;
  for (auto [dr, dc] : dirs) {
    const size_t jr = ir + dr;
    const size_t jc = ic + dc;
    // println("{},{}",jr,jc);
    if (chart.at(jr).at(jc) - chart.at(ir).at(ic) == 1) {
      ans += pathfind(chart, jr, jc);
    }
  }
  return ans;
}


void part1(const string &fn) {
  const vector<string> ip = aoc::pad(aoc::read_file(fn), '.', 1);
  int ans = 0;
  for (const auto &[ir, s] : ip | enumerate) {
    for (const auto [ic, c] : s | enumerate) {
      // println("{}",c);
      if (c == '0') {
        set<array<size_t,2>> visited;
        ans += pathfind(ip, ir, ic, visited);
      }
    }
  }
  println("day 10 part 1 ans = {}", ans);
}

void part2(const string &fn) {
  const vector<string> ip = aoc::pad(aoc::read_file(fn), '.', 1);
  int ans = 0;
  for (const auto &[ir, s] : ip | enumerate) {
    for (const auto [ic, c] : s | enumerate) {
      // println("{}",c);
      if (c == '0') {
        //set<array<size_t,2>> visited;
        ans += pathfind(ip, ir, ic);
      }
    }
  }
  println("day 10 part 2 ans = {}", ans);
}

int main() {
  part1("test_input");
  part1("input");
  part2("test_input");
  part2("input");
}

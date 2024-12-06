#include "../aoc.h"
#include <algorithm>
#include <array>
#include <numeric>
#include <print>
#include <set>
#include <string>
#include <tuple>
#include <vector>

using std::array, std::set, std::string, std::vector;
using std::tuple;
using std::println;

auto parse(const string &fn) { return aoc::pad(aoc::read_file(fn)); }

struct Guard {
  size_t irow, icol;
  int dr, dc;
  auto operator<=>(const Guard&) const = default;
};

Guard start(const auto ip) {
  for (size_t irow = 0; irow < ip.size(); irow++)
    for (size_t icol = 0; icol < ip.size(); icol++)
      if (ip.at(irow).at(icol) == '^')
        return Guard{irow, icol, -1, 0};
  throw std::runtime_error("no guard");
}

auto r(int dr, int dc) {
  //    0 +  + 0
  //    + 0  0 -
  //    0 -  - 0
  //    - 0  0 +
  return array{dc, -dr};
}

Guard walk(auto &grid, Guard &g) {
  auto [irow, icol, dr, dc] = g;
  size_t ir = irow + dr;
  size_t ic = icol + dc;
  if (grid.at(ir).at(ic) == '#') {
    std::tie(dr, dc) = r(g.dr, g.dc);
  }
  return Guard{irow + dr, icol + dc, dr, dc};
}

bool causes_loop(auto grid, Guard g, size_t obr, size_t obc) {
  set<Guard> visited;
  grid.at(obr).at(obc) = '#';
  while (grid.at(g.irow).at(g.icol) != '0') {
    if (visited.contains(g))
      return true;
    else
      visited.insert(g);
    g = walk(grid, g);
  }
  return false;
}

void day6(const string &fn) {
  const auto ip = parse(fn);
  Guard g = start(ip);
  auto grid = ip;
  while (grid.at(g.irow).at(g.icol) != '0') {
    // println("{},{} {},{} {} ", g.irow, g.icol, g.dr, g.dc,
    //         grid[g.irow][g.icol]);
    grid.at(g.irow).at(g.icol) = 'X';
    g = walk(grid, g);
  }

  const auto ans1 = std::transform_reduce(
      grid.begin(), grid.end(), 0lu, std::plus<>(),
      [](const string &s) -> size_t { return std::ranges::count(s, 'X'); });
  println("ans 1 = {}", ans1);

  int ans2 = 0;
  for (size_t r = 0; r < ip.size(); r++) {
    for (size_t c = 0; c < ip.at(r).size(); c++) {
      if (ip[r][c] == '.' && grid[r][c] == 'X') {
        if (causes_loop(grid, start(ip), r, c))
          ans2++;
      }
    }
  }
  println("ans 2 = {}", ans2);
}

int main() {
  day6("test_input");
  day6("input");
}

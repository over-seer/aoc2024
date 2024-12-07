#include "../aoc.h"
#include <algorithm>
#include <array>
#include <numeric>
#include <print>
#include <set>
#include <string>
#include <vector>

using std::array, std::set, std::string, std::vector;
using std::println;

auto parse(const string &fn) { return aoc::pad(aoc::read_file(fn)); }

struct Guard {
  size_t irow, icol;
  int dr, dc;
  auto operator<=>(const Guard &) const = default;
};

Guard start(const auto ip) {
  for (size_t irow = 0; irow < ip.size(); irow++)
    for (size_t icol = 0; icol < ip.size(); icol++)
      if (ip.at(irow).at(icol) == '^')
        return Guard{irow, icol, -1, 0};
  throw std::runtime_error("no guard");
}

auto r(int dr, int dc) { return array{dc, -dr}; }

Guard walk(auto &grid, Guard g) {
  while (grid.at(g.irow + g.dr).at(g.icol + g.dc) == '#') {
    std::tie(g.dr, g.dc) = r(g.dr, g.dc);
  }
  g.irow += g.dr;
  g.icol += g.dc;
  return g;
}

bool causes_loop(vector<string> grid, Guard g, size_t obr, size_t obc) {
  set<Guard> visited;
  grid.at(obr).at(obc) = '#';
  while (grid.at(g.irow).at(g.icol) != '0') {
    while (grid.at(g.irow + g.dr).at(g.icol + g.dc) == '#') {
      if (visited.contains(g))
        return true;
      else
        visited.insert(g);
      std::tie(g.dr, g.dc) = r(g.dr, g.dc);
    }
    g.irow += g.dr;
    g.icol += g.dc;
  }
  return false;
}

void day6(const string &fn) {
  const auto ip = parse(fn);
  Guard g = start(ip);
  auto grid = ip;
  while (grid.at(g.irow).at(g.icol) != '0') {
    grid.at(g.irow).at(g.icol) = 'X';
    g = walk(grid, g);
  }

  const auto ans1 = std::transform_reduce(
      grid.begin(), grid.end(), 0lu, std::plus<>(),
      [](const string &s) -> size_t { return std::ranges::count(s, 'X'); });
  println("ans 1 = {}", ans1);

  Guard g0 = start(ip);
  int ans2 = 0;
  for (size_t r = 1; r + 1 < ip.size(); r++) {
    for (size_t c = 1; c + 1 < ip.at(r).size(); c++) {
      if (grid[r][c] == 'X' && ip[r][c] == '.') {
        if (causes_loop(grid, g0, r, c)) {
          ans2++;
        }
      }
    }
  }
  println("ans 2 = {}", ans2);
}

int main() {
  day6("test_input");
  day6("input");
}

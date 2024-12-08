#include "aoc.h"
#include <array>
#include <map>
#include <print>
#include <set>
#include <string>
#include <vector>

using std::map, std::set, std::string, std::vector;
using std::println;

using Vec = std::array<int, 2>;

auto parse(const vector<string> &ip) {
  map<char, vector<Vec>> res;
  for (size_t row = 0; row < ip.size(); row++)
    for (size_t col = 0; col < ip.size(); col++) {
      const char c = ip[row][col];
      if (c != '.')
        res[c].push_back(Vec{static_cast<int>(row), static_cast<int>(col)});
    }
  return res;
}

auto get_antinodes(const vector<Vec> &ants, int nr, int nc) {
  set<Vec> antinodes;
  for (size_t ix0 = 0; ix0 < ants.size(); ++ix0) {
    for (size_t ix1 = ix0 + 1; ix1 < ants.size(); ++ix1) {
      auto [r0, c0] = ants[ix0];
      auto [r1, c1] = ants[ix1];
      auto dr = r1 - r0;
      auto dc = c1 - c0;
      const Vec new_pos1 = {r0 - dr, c0 - dc};
      const Vec new_pos2 = {r1 + dr, c1 + dc};
      for (const auto &[r, c] : {new_pos1, new_pos2})
        if (r >= 0 && r < nr && c >= 0 && c < nc)
          antinodes.insert(Vec{r, c});
    }
  }
  return antinodes;
}

auto get_antinodes_updated(const vector<Vec> &ants, int nr, int nc) {
  set<Vec> antinodes;
  for (size_t ix0 = 0; ix0 < ants.size(); ++ix0) {
    for (size_t ix1 = ix0 + 1; ix1 < ants.size(); ++ix1) {
      auto [r0, c0] = ants[ix0];
      auto [r1, c1] = ants[ix1];
      const auto dr = r1 - r0;
      const auto dc = c1 - c0;
      for (; r0 >= 0 && r0 < nr && c0 >= 0 && c0 < nc; r0 -= dr, c0 -= dc)
        antinodes.insert(Vec{r0, c0});
      for (; r1 >= 0 && r1 < nr && c1 >= 0 && c1 < nc; r1 += dr, c1 += dc)
        antinodes.insert(Vec{r1, c1});
    }
  }
  return antinodes;
}

void day8(const string &fn) {
  const auto ip = aoc::read_file(fn);
  const auto positions = parse(ip);
  set<Vec> unique_antinodes1, unique_antinodes2;
  for (auto [c, ants] : positions) {
    const auto antinodes1 = get_antinodes(ants, static_cast<int>(ip.size()),
                                          static_cast<int>(ip.at(0).size()));
    const auto antinodes2 = get_antinodes_updated(
        ants, static_cast<int>(ip.size()), static_cast<int>(ip.at(0).size()));
    aoc::merge(unique_antinodes1, antinodes1);
    aoc::merge(unique_antinodes2, antinodes2);
  }
  println("ans part 1 = {}", unique_antinodes1.size());
  println("ans part 2 = {}", unique_antinodes2.size());
}

int main() {
  day8("test_input");
  day8("input");
}
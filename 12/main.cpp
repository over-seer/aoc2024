#include "aoc.h"
#include <algorithm>
#include <print>

using std::array, std::set, std::string, std::vector;
using std::print, std::println;

using Pos = array<size_t, 2>;
using Dir = array<int, 2>;

struct Region {
  size_t area;
  size_t perimeter;
};

Region &operator+=(Region &r1, const Region &r2) {
  r1.area += r2.area;
  r1.perimeter += r2.perimeter;
  return r1;
}

Region find_region_recurs(const vector<string> &farm, char plot, const Pos &pos,
                          set<Pos> &assigned) {
  constexpr array<Dir, 4> dirs = {Dir{0, 1}, Dir{1, 0}, Dir{0, -1}, Dir{-1, 0}};
  const auto [ir, ic] = pos;
  if (assigned.contains(pos))
    return {0, 0};
  else
    assigned.insert(pos);
  Region r{.area = 1, .perimeter = 4};
  for (auto [dr, dc] : dirs) {
    const Pos next_pos = {ir + dr, ic + dc};
    const char neighbour = farm.at(next_pos[0]).at(next_pos[1]);
    if (neighbour == plot) {
      --r.perimeter;
      r += find_region_recurs(farm, plot, next_pos, assigned);
    }
  }
  return r;
}

size_t new_sides(const vector<string> &farm, Pos pos) {
  const auto [ir, ic] = pos;
  const char plot = farm.at(ir).at(ic);
  const bool n = farm.at(ir - 1).at(ic) != plot;
  const bool e = farm.at(ir).at(ic + 1) != plot;
  const bool s = farm.at(ir + 1).at(ic) != plot;
  const bool w = farm.at(ir).at(ic - 1) != plot;
  const bool ne = farm.at(ir - 1).at(ic + 1) != plot;
  const bool se = farm.at(ir + 1).at(ic + 1) != plot;
  const bool sw = farm.at(ir + 1).at(ic - 1) != plot;
  const bool nw = farm.at(ir - 1).at(ic - 1) != plot;
  size_t corners = 0;
  // outer corners
  if (n && e)
    ++corners;
  if (e && s)
    ++corners;
  if (s && w)
    ++corners;
  if (w && n)
    ++corners;
  // inner corners
  if (ne && !n && !e)
    ++corners;
  if (se && !s && !e)
    ++corners;
  if (sw && !s && !w)
    ++corners;
  if (nw && !n && !w)
    ++corners;
  return corners;
}

Region find_region_recurs2(const vector<string> &farm, char plot,
                           const Pos &pos, set<Pos> &assigned) {
  constexpr array<Dir, 4> dirs = {Dir{0, 1}, Dir{1, 0}, Dir{0, -1}, Dir{-1, 0}};
  const auto [ir, ic] = pos;
  if (assigned.contains(pos))
    return {0, 0};
  else
    assigned.insert(pos);
  Region r{.area = 1, .perimeter = new_sides(farm, pos)};
  // if(r.perimeter > 0) println("{}",r.perimeter);
  for (auto [dr, dc] : dirs) {
    const Pos next_pos = {ir + dr, ic + dc};
    const char neighbour = farm.at(next_pos[0]).at(next_pos[1]);
    if (neighbour == plot) {
      r += find_region_recurs2(farm, plot, next_pos, assigned);
    }
  }
  return r;
}

void part1(const string &fn) {
  const vector<string> ip = aoc::pad(aoc::read_file(fn), '.', 1);
  set<array<size_t, 2>> assigned;
  int ans = 0;
  for (size_t ir = 1; ir < ip.size() - 1; ir++) {
    for (size_t ic = 1; ic < ip.at(0).size() - 1; ic++) {
      const char id = ip.at(ir).at(ic);
      if (id != '.' && !assigned.contains({ir, ic})) {
        set<Pos> plots;
        auto [area, perimeter] = find_region_recurs(ip, id, {ir, ic}, plots);
        aoc::merge(assigned, plots);
        ans += area * perimeter;
      }
    }
  }
  println("day 12 part 1 ans = {}", ans);
}

void part2(const string &fn) {
  const vector<string> ip = aoc::pad(aoc::read_file(fn), '.', 1);
  set<array<size_t, 2>> assigned;
  int ans = 0;
  for (size_t ir = 1; ir < ip.size() - 1; ir++) {
    for (size_t ic = 1; ic < ip.at(0).size() - 1; ic++) {
      const char id = ip.at(ir).at(ic);
      if (id != '.' && !assigned.contains({ir, ic})) {
        set<Pos> plots;
        auto [area, sides] = find_region_recurs2(ip, id, {ir, ic}, plots);
        aoc::merge(assigned, plots);
        //println("{} {}", area, sides);
        ans += area * sides;
      }
    }
  }
  println("day 12 part 2 ans = {}", ans);
}

int main() {
  part1("test_input");
  part1("input");
  part2("test_input");
  part2("input");
}

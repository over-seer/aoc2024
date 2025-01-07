#include "aoc.h"
#include <cassert>
#include <print>
#include <ranges>
using std::array, std::set, std::map, std::string, std::vector;
using std::println, std::print;
using std::views::enumerate, std::ranges::join_view;

enum RowCol { ROW = 0, COL = 1 };
using Vec = array<int, 2>;
Vec &operator+=(Vec &v1, const Vec &v2) {
  v1[ROW] += v2[ROW];
  v1[COL] += v2[COL];
  return v1;
}

Vec operator+(Vec v1, const Vec &v2) {
  v1 += v2;
  return v1;
}

char get(const vector<string> &grid, const Vec &pos) {
  return grid.at(pos[ROW]).at(pos[COL]);
}

char &get_mut(vector<string> &grid, const Vec &pos) {
  return grid.at(pos[ROW]).at(pos[COL]);
}

Vec get_move(char c) {
  switch (c) {
  case '^':
    return {-1, 0};
    break;
  case '>':
    return {0, 1};
    break;
  case 'v':
    return {1, 0};
    break;
  case '<':
    return {0, -1};
    break;
  default:
    throw std::runtime_error("bad move");
  }
}

struct Input {
  vector<string> grid;
  vector<string> dirs;
};

Input parse(const string &fn) {
  const vector<vector<string>> ip = aoc::read_paragraph_ip(fn);
  assert(ip.size() == 2);
  return Input{ip[0], ip[1]};
}

bool is_stuck(vector<string> &grid, Vec pos) {
  auto [ir, ic] = pos;
  bool u = grid.at(ir - 1).at(ic) == '#';
  bool r = grid.at(ir).at(ic + 1) == '#';
  bool d = grid.at(ir + 1).at(ic) == '#';
  bool l = grid.at(ir).at(ic - 1) == '#';
  return (u && r) || (r && d) || (d && l) || (l && u);
}

set<Vec> check_stuck(vector<string> &grid) {
  set<Vec> stuck;
  for (auto &&[ir, s] : grid | enumerate) {
    for (auto &&[ic, ch] : s | enumerate) {
      if (ch == 'O' && is_stuck(grid, {static_cast<int>(ir), static_cast<int>(ic)}))
        get_mut(grid, {static_cast<int>(ir), static_cast<int>(ic)}) = '#';
      stuck.insert(Vec{static_cast<int>(ir), static_cast<int>(ic)});
    }
  }
  return stuck;
}

template <bool IsBox = false>
void step(vector<string> &grid, Vec &pos, const Vec dir) {
  const char c1 = get(grid, pos + dir);
  if (c1 == 'O') {
    Vec pos2 = pos + dir;
    step<true>(grid, pos2, dir);
  }

  const char c2 = get(grid, pos + dir);
  if (c2 == '.') {
    get_mut(grid, pos) = '.';
    pos += dir;
    if constexpr (IsBox) {
      get_mut(grid, pos) = 'O';
    }
    return;
  } else if (c2 == '#' || c2 == 'O') {
    return;
  } else
    throw std::runtime_error("bad char");
}

Vec start(const vector<string> &grid) {
  for (auto &&[ir, s] : grid | enumerate) {
    for (auto &&[ic, ch] : s | enumerate) {
      if (ch == '@')
        return Vec{static_cast<int>(ir), static_cast<int>(ic)};
    }
  }
  throw std::runtime_error("no robot!");
}

void part1(const string &fn) {
  auto [grid, moves] = parse(fn);
  Vec pos = start(grid);
  grid[pos[ROW]][pos[COL]] = '.';
  for (char c : join_view(moves)) {
    // println("{}", c);
    step(grid, pos, get_move(c));
  }
  int64_t ans = 0;
  for (auto &&[ir, s] : grid | enumerate) {
    for (auto &&[ic, ch] : s | enumerate) {
      if (ch == 'O')
        ans += 100 * ir + ic;
    }
  }
  println("part 1 ans = {}", ans);
}

auto stretch(const vector<string> &ip) {
  vector<string> op(ip.size());
  for (auto &&[ix, s1] : ip | enumerate) {
    string s2;
    for (char c : s1) {
      if (c == '#')
        s2 += "##";
      if (c == '.')
        s2 += "..";
      if (c == 'O')
        s2 += "[]";
      if (c == '@')
        s2 += "@.";
    }
    op.at(ix) = s2;
  }
  return op;
}

bool can_push_lr(vector<string> &grid, Vec pos, const Vec dir,
                 set<Vec> &to_push) {
  // const int ncol = static_cast<int>(grid.at(0).size());
  // const int nrow = static_cast<int>(grid.size());

  const int ir = pos[ROW];
  for (int ic = pos[COL];; ic += dir[COL]) {
    switch (grid[ir][ic]) {
    case '#':
      return false;
    case '.':
      return true;
    case '[':
    case ']':
      to_push.insert({ir, ic});
      break;
    default:
      println("{}", grid[ir][ic]);
      throw std::runtime_error("bad char");
    }
  }
}

bool can_push_ud(vector<string> &grid, Vec pos, const Vec dir,
                 set<Vec> &to_push) {
  const int dr = dir[ROW];
  const int ir = pos[ROW];
  const int ic0 = pos[COL];
  // println("{},{} ({})", ir, ic0, grid[ir][ic0]);
  const int ic1 = grid.at(ir).at(ic0) == '[' ? ic0 + 1 : ic0 - 1;
  const char cnext = grid.at(ir + dr).at(ic0);
  if (cnext == '#')
    return false;
  else {
    const bool can_push_this_side =
        (cnext == '.') || can_push_ud(grid, {ir + dr, ic0}, dir, to_push);
    if (can_push_this_side) {
      to_push.insert(pos);
      const bool can_push_that_side =
          to_push.contains({ir, ic1}) ||
          can_push_ud(grid, {ir, ic1}, dir, to_push);
      return can_push_that_side;
    }
    return false;
  }
  return false;
}

bool can_push(vector<string> &grid, Vec pos, const Vec dir, set<Vec> &to_push) {
  return dir[ROW] == 0 ? can_push_lr(grid, pos, dir, to_push)
                       : can_push_ud(grid, pos, dir, to_push);
}

void update_grid(vector<string> &grid, Vec dir, const set<Vec> &to_push) {
  map<Vec, char> chmap;
  for (auto [ir, ic] : to_push) {
    chmap[{ir, ic}] = grid[ir][ic];
    grid[ir][ic] = '.';
  }
  for (auto [pos, ch] : chmap) {
    const int ir = pos[ROW] + dir[ROW];
    const int ic = pos[COL] + dir[COL];
    grid[ir][ic] = ch;
    // println("{},{} = {}", ir, ic, ch);
  }
}

void step2(vector<string> &grid, Vec &pos, const Vec dir) {
  const char c = get(grid, pos + dir);
  if (c == '.') {
    pos += dir;
    return;
  } else if (c == '#') {
    return;
  } else if (c == '[' || c == ']') {
    set<Vec> to_push;
    if (can_push(grid, pos + dir, dir, to_push)) {
      // println("debig can push!");
      pos += dir;
      to_push.insert(pos);
      update_grid(grid, dir, to_push);
    }
  } else {
    println("{}", c);
    throw std::runtime_error("bad char");
  }
}

void part2(const string &fn) {
  auto [oldgrid, moves] = parse(fn);
  auto grid = stretch(oldgrid);
  Vec pos = start(grid);
  grid[pos[ROW]][pos[COL]] = '.';
  for (char c : join_view(moves)) {
    grid[pos[ROW]][pos[COL]] = '.';
    step2(grid, pos, get_move(c));
    // println("Move {}", c);
    grid[pos[ROW]][pos[COL]] = '@';
    // for (auto s : grid)      println("{}", s);
  }
  int64_t ans = 0;
  for (auto &&[ir, s] : grid | enumerate) {
    for (auto &&[ic, ch] : s | enumerate) {
      if (ch == '[')
        ans += 100 * ir + ic;
    }
  }
  println("part 1 ans = {}", ans);
}

int main() {
  part1("test_input");
  part1("input");
  part2("test_input_small");
  part2("test_input");
  part2("input");
}
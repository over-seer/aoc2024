#include "aoc.h"
#include <print>
#include <ranges>
using namespace std;

enum XY { ROW, COL };
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
using Maze = vector<string>;

auto r(int dr, int dc) { return array{dc, -dr}; }
auto l(int dr, int dc) { return array{-dc, dr}; }

vector<Vec> path;
set<Vec> seats;

void recurse(const Maze &maze, Vec pos, Vec dir, Vec target, int nstep,
             int ncorner, int &best, bool corner, auto &visited) {
  // println("{},{} {},{} {}", pos[ROW], pos[COL],
  // dir[ROW],dir[COL],maze[pos[ROW]][pos[COL]]);
  path.push_back(pos);
  const int score = nstep + 1000 * ncorner;
  if (score > best) {
    path.pop_back();
    return;
  }
  if (pos == target) {
    // if (score < best) println("nstep {} ncorner {} score {}", nstep, ncorner,
    // score);
    if(score < best) seats.clear();
    if(score <= best) for(auto p : path) seats.insert(p);
    best = min(best, score);
    path.pop_back();
    return;
  }

  if (corner) {
    // println("corner! {},{} {},{} {}", pos[ROW],
    // pos[COL],dir[ROW],dir[COL],maze[pos[ROW]][pos[COL]]);
    if (visited.contains({pos, dir})) {
      auto [ns, nc] = visited.at({pos, dir});
      if (nstep >= ns && ncorner > nc) {
        path.pop_back();
        return;
      }
    }
    visited[{pos, dir}] = {nstep, ncorner};
  }
  const auto [ir, ic] = pos;
  for (auto [dr, dc] : {dir, l(dir[ROW], dir[COL]), r(dir[ROW], dir[COL])}) {
    if (maze.at(ir + dr).at(ic + dc) != '#') {
      const bool is_corner = dr != dir[ROW];
      int nc = is_corner ? ncorner + 1 : ncorner;
      recurse(maze, {ir + dr, ic + dc}, {dr, dc}, target, nstep + 1, nc, best,
              is_corner, visited);
    }
  }
  path.pop_back();
}

void part1(const string &fn) {
  const vector<string> ip = aoc::read_file(fn);
  const int n = static_cast<int>(ip.size());
  // int best = n * n * n * n;
  //int best = 1573696;
  int best = 93437;
  set<Vec> visited;
  map<pair<Vec, Vec>, pair<int, int>> tally;
  recurse(ip, {n - 2, 1}, {0, 1}, {1, n - 2}, 0, 0, best, false, tally);
  auto ip2 = ip;
  for (auto [ir, ic] : seats) {
    ip2[ir][ic] = 'O';
  }
  for (auto s : ip2)
    println("{}", s);
  println("day 16 part 1 ans = {}", best);
  println("day 16 part 2 ans = {}", seats.size());
  const int ncorner = best / 1000;
  const int nsteps = best % 1000;
  
}

int main() {
  part1("test_input2");
  part1("input");
}
#include "aoc.h"
#include <print>
#include <ranges>

using namespace std;

template <int N> struct Grid {
  array<array<int, N>, N> a;
  int &operator[](int i1, int i2) { return a[i1][i2]; }
  const int &operator[](int i1, int i2) const { return a[i1][i2]; }
  int *begin() { return &a[0][0]; }
  int *end() { return &a[N][0]; }
};

enum RowCol { ROW = 0, COL = 1 };
using Vec = array<int, 2>;
Vec &operator+=(Vec &v1, const Vec &v2) {
  v1[ROW] += v2[ROW];
  v1[COL] += v2[COL];
  return v1;
}

template <int N> struct Ip {
  vector<string> maze;
  Vec start, end;
};

template <int N> Ip<N> parse(const string &fn) {
  Ip<N> ip;
  ip.maze = aoc::read_file(fn);
  for (const auto &[ir, row] : ip.maze | views::enumerate) {
    for (const auto [ic, ch] : row | views::enumerate) {
      if (ch == 'S')
        ip.start = Vec{static_cast<int>(ir), static_cast<int>(ic)};
      else if (ch == 'E')
        ip.end = Vec{static_cast<int>(ir), static_cast<int>(ic)};
    }
  }
  return ip;
}

template <int N>
int dijkstra(const auto &walls, Grid<N> &counts, const Vec pos,
             const Vec target, int nsteps) {
  constexpr array dirs = {Vec{0, 1}, Vec{1, 0}, Vec{0, -1}, Vec{-1, 0}};
  if (pos == target)
    return nsteps;
  int best = N * N;
  const auto &[ir, ic] = pos;
  counts[ir, ic] = nsteps;
  nsteps++;
  for (auto [dr, dc] : dirs) {
    if (counts[ir + dr, ic + dc] > nsteps &&
        walls.at(ir + dr).at(ic + dc) != '#') {
      best = min(
          best, dijkstra(walls, counts, Vec{ir + dr, ic + dc}, target, nsteps));
    }
  }
  return best;
}

template <int N, int M> auto diamond(int ir, int ic) {
  vector<Vec> op;
  for (int dr = -M; dr <= M; dr++) {
    const int adr = abs(dr);
    for (int dc = -M + adr; dc <= (M - adr); dc++) {
      const int jr = ir + dr;
      const int jc = ic + dc;
      if (jr >= 0 && jr < N && jc >= 0 && jc < N) {
        op.push_back(Vec{jr, jc});
      }
    }
  }
  return op;
}

void diamond_test() {
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      vector<string> v(10, string(10, '.'));
      const auto iijj = diamond<10, 3>(i, j);
      for (auto [ii, jj] : iijj) {
        v.at(ii).at(jj) = '#';
      }

      for (auto &s : v)
        println("{}", s);
      println("\n\n");
    }
  }
}

template <int N> void day20(const string &fn) {
  const Ip<N> ip = parse<N>(fn);
  Grid<N> fwd, bck;
  ranges::fill(fwd, N * N);
  ranges::fill(bck, N * N);
  const int best = dijkstra(ip.maze, fwd, ip.start, ip.end, 0);
  assert(best == dijkstra(ip.maze, bck, ip.end, ip.start, 0));
  int ans1 = 0;
  map<int, int> tally;
  for (int ir = 0; ir < N; ir++) {
    for (int ic = 0; ic < N; ic++) {
      for (auto [dr, dc] : {Vec{0, -2}, Vec{-2, 0}}) {
        const int ir1 = ir + dr;
        const int ic1 = ic + dc;
        if ((ir1) > 0 && (ic1) > 0 && ip.maze.at(ir).at(ic) != '#' &&
            ip.maze.at(ir1).at(ic1) != '#') {
          int len1 = fwd[ir, ic] + bck[ir1, ic1] + 2;
          int len2 = bck[ir, ic] + fwd[ir1, ic1] + 2;
          int len = min(len1, len2);
          // if (len < best) ++tally[best - len];
          if ((best - len) >= 100)
            ans1++;
        }
      }
    }
  }
  // for (auto [d, n] : tally) println("{} : {}", d, n);
  println("part 1 ans = {}", ans1);
  tally.clear();
  set<array<Vec, 2>> pts;

  int ans2 = 0;
  for (int ir0 = 0; ir0 < N; ir0++) {
    for (int ic0 = 0; ic0 < N; ic0++) {
      if (ip.maze.at(ir0).at(ic0) != '#') {
        for(auto [ir1,ic1] : diamond<N,20>(ir0,ic0)) {
          if (ip.maze.at(ir1).at(ic1) != '#') {
            const int md = abs(ir1 - ir0) + abs(ic1 - ic0);
            const int len = fwd[ir0, ic0] + bck[ir1, ic1] + md;
            if (len < best)
              ++tally[best - len];
            if ((best - len) >= 100)
              ans2++;
          }
        }
      }
    }
  }

  //for (auto [d, n] : tally) println("{} : {}", d, n);
  println("part 2 ans = {}", ans2);
}

int main() {
  //day20<15>("test_input");
  day20<141>("input");
  //diamond_test();
}

#include "aoc.h"
#include <print>
#include <ranges>

using namespace std;

using Vec = array<int, 2>;

auto parse(const string &fn) {
  auto getvec = [](const string &s) -> array<int, 2> {
    auto [s1, s2] = aoc::splitn<2>(s,',');
    //println("{} {}",s1,s2);
    return Vec{stoi(s2), stoi(s1)};
  };
  auto ip = aoc::read_file(fn) | views::transform(getvec);
  return vector<Vec>(ip.begin(), ip.end());
}

template <int N> struct Mem {
  array<array<int, N>, N> a;
  int &operator[](int i1, int i2) { return a[i1][i2]; }
  const int &operator[](int i1, int i2) const { return a[i1][i2]; }
  int *begin() { return &a[0][0]; }
  int *end() { return &a[N][0]; }
};

constexpr array dirs = {Vec{0, 1}, Vec{1, 0}, Vec{0, -1}, Vec{-1, 0}};

template <int N> void dijkstra(Mem<N> &mem, Vec pos, int nsteps) {
  auto [i, j] = pos;
  if (i < N && i >= 0 && j < N && j >= 0 && mem[i, j] > nsteps) {
    mem[i, j] = nsteps;
    if (i == N - 1 && j == N - 1)
      return;
    nsteps++;
    for (auto [di, dj] : dirs) {
      dijkstra(mem, {i + di, j + dj}, nsteps);
    }
  }
  return;
}

template <int N> void part1(const string &fn, int bytes) {
  auto ip = parse(fn);
  Mem<N> mem;
  ranges::fill(mem, N * N);
  for (auto [i, j] : ip | views::take(bytes))
    mem[i, j] = -1;
  //for(auto row : mem.a) {
  //  for(auto i : row) print("{}", i < 0 ? '#' : '.');
  //  println();
  //}
  auto mem1 = mem;
  dijkstra(mem1, {0, 0}, 0);
  println("part 1 ans = {}", mem1[N - 1, N - 1]);
  for(auto [i,j] : ip | views::drop(bytes)) {
    //println("{},{}",j,i);
    mem[i,j] = -1;
    auto mem2 = mem;
    dijkstra(mem2, {0, 0}, 0);
    if(mem2[N-1,N-1] == N*N) {
        println("part 2 ans = {},{}", j,i);
        break;
    }
  }
}

int main() {
  part1<7>("test_input",12);
  part1<71>("input",1024);
}
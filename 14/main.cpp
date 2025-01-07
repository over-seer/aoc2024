#include "aoc.h"
#include <print>
#include <ranges>

using std::array, std::string, std::vector;
using std::print, std::println;
namespace views = std::views;
enum XY { ROW = 0, COL = 1 };
using Vec = array<int, 2>;
struct Robot {
  Vec p;
  Vec v;
};

auto parse(const string &fn) {
  auto vecparse = [](const string &s) {
    auto v = aoc::splitn<2, ','>(s.substr(2));
    // println("{} {}",v.at(0),v.at(1));
    return Vec{stol(v.at(1)), stol(v.at(0))};
  };
  vector<Robot> op;
  for (auto &&[spos, sdir] :
       aoc::read_file(fn) | std::views::transform(aoc::splitn<2, ' '>)) {
    op.push_back(Robot{vecparse(spos), vecparse(sdir)});
  }
  return op;
}

int nsec(int i, int d, int sz, int n) {
  int j = (i + n * d) % sz;
  return (j + sz) % sz;
}

template <int NROW, int NCOL> void part1(const string &fn) {
  auto ip = parse(fn);
  //for (auto [p, v] : ip) println("{},{} {},{}", p[ROW], p[COL], v[ROW], v[COL]);
  array quads = {0, 0, 0, 0};
  vector<string> pic(NROW, string(NCOL, '.'));
  for (auto [p, v] : ip) {
    const int irow = nsec(p[ROW], v[ROW], NROW, 100);
    const int icol = nsec(p[COL], v[COL], NCOL, 100);

    pic[irow][icol] = '#';
    if (irow < NROW / 2 && icol < NCOL / 2)
      ++quads[0];
    if (irow < NROW / 2 && icol > NCOL / 2)
      ++quads[1];
    if (irow > NROW / 2 && icol < NCOL / 2)
      ++quads[2];
    if (irow > NROW / 2 && icol > NCOL / 2)
      ++quads[3];
  }
  
  auto ans = quads[0] * quads[1] * quads[2] * quads[3];
  println("ans part 1 = {}", ans);
}

template <size_t N> void print_tally(const array<size_t, N> &tally) {
  const auto maxn = std::ranges::max(tally);
  vector<string> op(maxn, string(N, '.'));
  for (auto [ix, n] : tally | std::views::enumerate) {
    for (size_t j = 0; j < n; j++) {
      op[maxn - j - 1][ix] = '#';
    }
  }
  for (const auto &s : op)
    println("{}", s);
}

template <size_t N>
auto get_tally(const auto &pv, int nstep) -> array<size_t, N> {
  array<size_t, N> tally{};
  for (auto [p, v] : pv) {
    ++tally.at(nsec(p, v, N, nstep));
  }
  return tally;
}

auto get_row(const Robot &r) { return array{r.p[ROW], r.v[ROW]}; }
auto get_col(const Robot &r) { return array{r.p[COL], r.v[COL]}; }

template <int NROW, int NCOL> auto tallies(const string &fn) -> array<int,2> {
  const auto ip = parse(fn);
  auto rows = ip | std::views::transform(get_row);
  auto cols = ip | std::views::transform(get_col);
  int rowoff = 0;
  int coloff = 0;
  int rowmax = 0;
  int colmax = 0;
  for (int i = 0; i < NCOL * 2; i++) {
    const auto tally = get_tally<NCOL>(cols, i);
    const int mx = std::ranges::max(tally);
    if(mx > rowmax) {
      rowmax = mx;
      rowoff = i;
    }
    //print_tally(tally);
    //println("max = {}",mx);
  }
  for (int i = 0; i < NROW * 2; i++) {
    const auto tally = get_tally<NROW>(rows, i);
    const int mx = std::ranges::max(tally);
    if(mx > colmax) {
      colmax = mx;
      coloff = i;
    }
    //print_tally(tally);
    //println("max = {}",mx);
  }
  println("{} {}",rowoff,coloff);
  return {rowoff,coloff};
}

template <int NROW, int NCOL> void part2_ans(int off1, int off2) {
  for (int i = off1; true; i += NROW) {
    if (i % NCOL == off2) {
      println("ans part 2 = {}", i);
      return;
    }
  }
}

int main() {
  part1<103, 101>("input");
  // part1<7, 11>("test_input");
  //array<size_t, 5> tally = {0, 8, 0, 11, 1};
  //print_tally(tally);
  auto [rowoff,coloff] = tallies<103, 101>("input");
  part2_ans<103, 101>(coloff,rowoff);
}
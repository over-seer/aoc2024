#include "aoc.h"
#include <functional>
#include <numeric>
#include <print>
#include <string>
#include <vector>

using std::println;
using std::string, std::vector;
using std::ranges::transform;

auto parse(const string &fn) {
  const auto ip = aoc::read_file(fn);
  vector<vector<int64_t>> vvi;
  vvi.reserve(ip.size());
  auto stovi = [](const string &s) {
    auto vs = aoc::split(s, ' ');
    vector<int64_t> v;
    v.reserve(vs.size());
    vs.at(0) = vs.at(0).substr(0, vs.at(0).size() - 1);
    transform(vs, back_inserter(v), [](const string &s) { return stol(s); });
    return v;
  };
  transform(ip, back_inserter(vvi), stovi);
  return vvi;
}

int64_t concat(int64_t x, int64_t y) {
  int64_t pow = 10;
  while (y >= pow)
    pow *= 10;
  return x * pow + y;
}

template <bool Concatenate = false>
int find_solutions_recurs(const vector<int64_t> &v, size_t ix, int64_t result) {
  if (ix + 1 == v.size()) {
    return result == v[0] ? 1 : 0;
  }

  if (ix == 1)
    result = v[1];
  int solution =
      find_solutions_recurs<Concatenate>(v, ix + 1, result + v[ix + 1]) +
      find_solutions_recurs<Concatenate>(v, ix + 1, result * v[ix + 1]);
  if constexpr (Concatenate)
    solution += find_solutions_recurs<Concatenate>(v, ix + 1,
                                                   concat(result, v[ix + 1]));
  return solution;
}

void day7(const string &fn) {
  auto ip = parse(fn);
  uint64_t ans1 = 0;
  for (const auto &v : ip)
    if (find_solutions_recurs(v, 1, 0) > 0) {
      ans1 += v[0];
    }
  println("ans 1 = {}", ans1);

  uint64_t ans2 = 0;
  for (const auto &v : ip)
    if (find_solutions_recurs<true>(v, 1, 0) > 0) {
      ans2 += v[0];
    }
  println("ans 2 = {}", ans2);
}

int main() {
  //println("{}", concat(23, 456));
  day7("test_input");
  day7("input");
}
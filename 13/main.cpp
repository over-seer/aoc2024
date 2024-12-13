#include "aoc.h"

#include <array>
#include <optional>
#include <print>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

using std ::array, std::optional, std::pair, std::string, std::vector;
using std::print, std::println;

enum XY { X, Y };
using Vec = array<int64_t, 2>;
struct Claw {
  Vec a;
  Vec b;
  Vec prize;
};

Claw make_claw(const vector<string> &v) {
  auto dir = [](const string &s) {
    return Vec{stol(s.substr(12, 2)), stol(s.substr(18, 2))};
  };
  auto xpos = v.at(2).find("X");
  auto ypos = v.at(2).find("Y");
  int64_t x = stol(v.at(2).substr(xpos + 2, ypos - xpos - 2));
  int64_t y = stol(v.at(2).substr(ypos + 2));
  return {.a = dir(v.at(0)), .b = dir(v.at(1)), .prize = {x, y}};
}

auto parse(const string &fn) {
  auto vw = aoc::read_paragraph_ip(fn) | std::views::transform(make_claw);
  return vector<Claw>(vw.begin(), vw.end());
}

void printclaw(Claw claw) {
  println("Button A: X+{} Y+{}", claw.a[X], claw.a[Y]);
  println("Button B: X+{} Y+{}", claw.b[X], claw.b[Y]);
  println("Prize: X={} Y={}", claw.prize[X], claw.prize[Y]);
  println();
}

optional<pair<int64_t, int64_t>> cramer(int64_t a1, int64_t b1, int64_t c1,
                                        int64_t a2, int64_t b2, int64_t c2) {
  const int64_t topa = (c1 * b2 - b1 * c2);
  const int64_t bttma = (a1 * b2 - b1 * a2);
  const int64_t topb = (a1 * c2 - c1 * a2);
  const int64_t bttmb = (a1 * b2 - b1 * a2);
  if (topa % bttma || topb % bttmb)
    return std::nullopt;
  return std::make_pair((c1 * b2 - b1 * c2) / (a1 * b2 - b1 * a2),
                        (a1 * c2 - c1 * a2) / (a1 * b2 - b1 * a2));
}

auto solve(const Claw &c) {
  return cramer(c.a[X], c.b[X], c.prize[X], c.a[Y], c.b[Y], c.prize[Y]);
}

void part1(const string &fn) {
  const vector<Claw> ip = parse(fn);
  // for(auto c : ip) printclaw(c);
  int64_t ans = 0;
  for (auto c : ip) {
    if (const auto result = solve(c); result.has_value()) {
      const auto [na, nb] = *result;
      // println("na = {}, nb = {}",na,nb);
      ans += 3 * na + nb;
    }
  }
  println("day 13 part 1 ans = {}", ans);
}

void part2(const string &fn) {
  const auto ip = parse(fn) | std::views::transform([](Claw c) {
                    c.prize[X] += 10000000000000;
                    c.prize[Y] += 10000000000000;
                    return c;
                  });
  int64_t ans = 0;
  for (auto c : ip) {
    if (const auto result = solve(c); result.has_value()) {
      const auto [na, nb] = *result;
      ans += 3 * na + nb;
    }
  }
  println("day 13 part 1 ans = {}", ans);
}

int main() {
  part1("test_input");
  part1("input");
  part2("input");
}

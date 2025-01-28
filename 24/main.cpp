#include "aoc.h"
#include <functional>
#include <print>
#include <ranges>
using namespace std;
// namespace ranges = std::ranges;
// namespace views = std::ranges::views;

struct Rule {
  string a, b;
  std::function<bool(bool, bool)> op;
};

auto parse(const string &fn) -> pair<map<string, bool>, map<string, Rule>> {
  const auto ip = aoc::read_paragraph_ip(fn);
  auto pt1 = [](const string &s) {
    return make_pair(s.substr(0, 3), s[5] == '1');
  };
  auto get_op = [](const string &s) -> std::function<bool(bool, bool)> {
    if (s == "OR")
      return logical_or<bool>();
    else if (s == "AND")
      return logical_and<bool>();
    else if (s == "XOR")
      return not_equal_to<bool>();
    else
      throw runtime_error("bad operation");
  };
  auto pt2 = [&](const string &s) {
    const auto words = aoc::split(s, ' ');
    return make_pair(
        words[4], Rule{.a = words[0], .b = words[2], .op = get_op(words[1])});
  };
  return make_pair(ip[0] | views::transform(pt1) | ranges::to<map>(),
                   ip[1] | views::transform(pt2) | ranges::to<map>());
}

bool get_bit(const map<string, Rule> &rules, map<string, bool> &bits,
             const string &key) {
  if (bits.contains(key))
    return bits.at(key);
  else {
    // println("debug {}",key);
    const auto &[a, b, op] = rules.at(key);
    const bool bit = op(get_bit(rules, bits, a), get_bit(rules, bits, b));
    bits[key] = bit;
    return bit;
  }
}

void part1(const string &fn) {
  auto [bits, rules] = parse(fn);
  // for(auto [k,v] : bits) println("{} {}",k,v);
  const auto ans =
      rules | views::keys |
      views::filter([](const auto &k) { return k.front() == 'z'; }) |
      views::transform(
          [&](const auto &k) { return get_bit(rules, bits, k) ? '1' : '0'; }) |
      views::reverse | ranges::to<string>();
  println("{} day 24 part 1 ans = {}", fn, stoull(ans, nullptr, 2));
}

int main() {
  part1("test_input");
  part1("input");
}
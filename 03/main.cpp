#include "../aoc.h"
#include <print>
#include <regex>
#include <string>

using std::println;
using std::regex, std::smatch, std::sregex_iterator;
using std::string, std::vector;

const vector<string> test_input = {
    "xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))"};
const vector<string> test_input2 = {
    "xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))"};
const regex mul_regex("mul\\([0-9]+,[0-9]+\\)");
const regex mul_regex2("((mul\\([0-9]+,[0-9]+\\))|(do\\(\\))|(don't\\(\\)))");

int64_t product(const string &s) {
  const string snums = s.substr(4, s.size() - 5);
  const vector<string> v = aoc::split(snums, ',');
  const int64_t x = stoi(v.at(0));
  const int64_t y = stoi(v.at(1));
  return x * y;
}

void part1(const vector<string> &ip) {
  // printf("%s\n",mem.c_str());
  int64_t ans = 0;
  for (auto &mem : ip) {

    smatch mul_match;
    auto mul_begin = sregex_iterator(mem.begin(), mem.end(), mul_regex);
    auto mul_end = sregex_iterator();
    for (sregex_iterator i = mul_begin; i != mul_end; ++i) {
      smatch match = *i;
      string match_str = match.str();
      ans += product(match_str);
    }
  }
  println("part1 = {}", ans);
}

int64_t handle(const vector<string> &instr) {
  int64_t ans = 0;
  bool is_do = true;
  for (const auto &s : instr) {
    if (s == "do()")
      is_do = true;
    else if (s == "don't()")
      is_do = false;
    else if (is_do)
      ans += product(s);
  }
  return ans;
}

void part2(const vector<string> &ip) {
  vector<string> instructions;

  int64_t ans = 0;
  for (auto &mem : ip) {

    smatch mul_match;
    auto mul_begin = sregex_iterator(mem.begin(), mem.end(), mul_regex2);
    auto mul_end = sregex_iterator();
    for (sregex_iterator i = mul_begin; i != mul_end; ++i) {
      smatch match = *i;
      string match_str = match.str();
      instructions.push_back(match_str);
    }
  }
  ans = handle(instructions);
  println("part 2 = {}", ans);
}

int main() {
  part1(test_input);
  part1(aoc::read_file("input"));
  part2(test_input2);
  part2(aoc::read_file("input"));
}
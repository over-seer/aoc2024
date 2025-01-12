#include "aoc.h"
#include <print>
#include <ranges>
using namespace std;

auto parse(const string &fn) {
  auto ip = aoc::read_file(fn);
  const int a = stoi(ip[0].substr(12));
  const auto program = ip[4].substr(9) | views::split(',') |
                       views::transform([](auto s) -> int {
                         return stoi(string{string_view{s}});
                       }) |
                       ranges::to<vector>();
  return make_pair(a, program);
}

struct Comp {
  enum Instr : size_t {
    ADV = 0,
    BXL = 1,
    BST = 2,
    JNZ = 3,
    BXC = 4,
    OUT = 5,
    BDV = 6,
    CDV = 7
  };

  int a{};
  int b{};
  int c{};
  vector<int> program;
  vector<int> op;
  size_t ix{};
  int combo(int o) {
    switch (o) {
    case 0:
    case 1:
    case 2:
    case 3:
      return o;
    case 4:
      return a;
    case 5:
      return b;
    case 6:
      return c;
    default:
      throw runtime_error("bad combo operand");
    }
  }

  void output(int i) {
    op.push_back(i);
    // print("{},", i);
  }

  bool next_instruction() {
    if (ix + 1 >= program.size())
      return false;
    const int instruction = program[ix];
    const int operand = program.at(ix + 1);
    // println("ix = {} instruction = {} operand = {}", ix, instruction,
    // operand);
    switch (instruction) {
    case ADV:
      a = a / (1 << combo(operand));
      ix = ix + 2;
      break;
    case BXL:
      b = b ^ operand;
      ix = ix + 2;
      break;
    case BST:
      b = combo(operand) % 8;
      ix = ix + 2;
      break;
    case JNZ:
      if (a != 0) {
        ix = operand;
      } else
        ix = ix + 2;
      break;
    case BXC:
      b = b ^ c;
      ix = ix + 2;
      break;
    case OUT:
      output(combo(operand) % 8);
      ix = ix + 2;
      break;
    case BDV:
      b = a / (1 << combo(operand));
      ix = ix + 2;
      break;
    case CDV:
      c = a / (1 << combo(operand));
      ix = ix + 2;
      break;
    default:
      throw runtime_error("bad instruction");
    }
    return true;
  }

  void run() {
    bool keep_going = true;
    do {
      keep_going = next_instruction();
    } while (keep_going);
  }
};

void part1(const string &fn) {
  auto [a, program] = parse(fn);
  println("{}", a);
  for (auto i : program)
    println("{}", i);
  Comp comp{.a = a, .program = program};
  comp.run();
  for (auto i : comp.op)
    print("{},", i);
  println();
}

int main() {
  part1("test_input");
  part1("input");
}
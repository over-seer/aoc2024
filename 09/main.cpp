#include "aoc.h"
#include <map>
#include <optional>
#include <print>
#include <string>
#include <vector>
#include <ranges>

using std::format, std::print, std::println, std::stoul;
using std::map, std::optional, std::string, std::vector;
using std::views::reverse, std::views::enumerate;

const string test_input = "2333133121414131402";

auto get_blocks(const string s) {
  vector<optional<size_t>> v;
  for (size_t ix = 0; ix < s.size(); ix++) {
    const size_t id = ix / 2;
    const size_t n = stoul(format("{}", s[ix]));
    if (ix % 2 == 0) {
      v.insert(v.end(), n, id);
    } else {
      v.insert(v.end(), n, std::nullopt);
    }
  }
  return v;
}

auto order_blocks(vector<optional<size_t>> blocks) {
  vector<size_t> ordered;

  size_t iback = blocks.size();
  for (size_t ifront = 0; ifront < blocks.size() && ifront < iback; ++ifront) {
    if (const auto el = blocks[ifront]; el.has_value()) {
      ordered.push_back(*el);
    } else {
      while (!blocks.at(--iback).has_value()) {
      }
      ordered.push_back(blocks.at(iback).value());
    }
  }
  return ordered;
}

void part1(const string &s) {
  const auto blocks = get_blocks(s);
  const auto ordered = order_blocks(blocks);
  size_t ans1 = 0;
  for (size_t ix = 0; ix < ordered.size(); ix++)
    ans1 += ix * ordered[ix];
  println("part 1 ans = {}", ans1);
}

struct File {
  size_t pos, len;
};

void part2(const string &s) {
  map<size_t, size_t> gaps; // key = pos, value = len
  vector<File> files;       // key = id, value = {pos,len}
  size_t pos = 0;
  for (size_t ix = 0; ix < s.size(); ix++) {
    const size_t n = stoul(format("{}", s[ix]));
    if (ix % 2 == 0) {
      files.push_back({pos, n});
    } else {
      gaps[pos] = n;
    }
    pos += n;
  }

  for (auto && [id,file] : files | enumerate | reverse) {
    const size_t reqd_len = file.len;
    const size_t current_pos = file.pos;
    for (auto it = gaps.begin(); it != gaps.end(); ++it) {
      auto [pos, len] = *it;
      if (pos > current_pos)
        break;
      if (len >= reqd_len) {
        file.pos = pos;
        gaps.erase(it);
        if (len != reqd_len)
          gaps[pos + reqd_len] = len - reqd_len;
        break;
      }
    }
  }
  auto sum_natural = [](size_t n) { return n * (n + 1) / 2; };
  size_t ans2 = 0;
  for (size_t id = 0; id < files.size(); id++) {
    // println("id {} pos {} len {}", id, files[id].pos, files[id].len);
    auto [pos, len] = files[id];
    ans2 += (sum_natural(pos + len - 1) - sum_natural(pos - 1)) * id;
  }
  println("part 2 ans = {}", ans2);
}

int main() {
  part1(test_input);
  part1(aoc::read_file("input").at(0));
  part2(test_input);
  part2(aoc::read_file("input").at(0));
}

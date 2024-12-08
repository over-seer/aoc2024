#include "aoc.h"
#include <array>
#include <print>
#include <stdexcept>
#include <string>
#include <vector>

using std::array, std::string, std::vector;
using std::println;

const vector<string> test = {
    "MMMSXXMASM", "MSAMXMSMSA", "AMXSXMAAMM", "MSAMASMSMX", "XMASAMXAMM",
    "XXAMMXXAMA", "SMSMSASXSS", "SAXAMASAAA", "MAMMMXMMMM", "MXMXAXMASX"};

constexpr array dirs = {array{-1, -1}, array{-1, 0}, array{-1, 1}, array{0, -1},
                        array{0, 1},   array{1, -1}, array{1, 0},  array{1, 1}};

int num_matches(const vector<string> &ip, const string &word, size_t irow,
                size_t icol) {
  int sum = 0;
  if (ip.at(irow).at(icol) != word[0])
    return 0;

  for (auto [dr, dc] : dirs) {
    size_t ir = irow;
    size_t ic = icol;
    bool is_match = true;
    for (size_t iletter = 1; iletter < word.size(); iletter++) {
      ir += dr;
      ic += dc;
      if (word[iletter] != ip.at(ir).at(ic)) {
        is_match = false;
        break;
      }
    }
    if (is_match)
      sum++;
  }
  return sum;
}

bool mands(char a, char b) {
  return (a == 'M' && b == 'S') || (a == 'S' && b == 'M');
}

bool is_xmas(const vector<string> &ip, size_t irow, size_t icol) {
  if (ip.at(irow).at(icol) == 'A') {
    char tl = ip.at(irow - 1).at(icol - 1);
    char tr = ip.at(irow - 1).at(icol + 1);
    char bl = ip.at(irow + 1).at(icol - 1);
    char br = ip.at(irow + 1).at(icol + 1);
    if (mands(tl, br) && mands(tr, bl))
      return true;
  }
  return false;
}

void part1(const vector<string> &ip, const string &word) {
  const size_t n = word.size() - 1;
  const auto padded = aoc::pad(ip, '0', n);
  int ans = 0;
  for (size_t irow = n; irow < padded.size() - n; ++irow) {
    for (size_t icol = n; icol < padded[irow].size() - n; ++icol) {
      ans += num_matches(padded, word, irow, icol);
    }
  }
  println("ans part1 = {}", ans);
}

void part2(const vector<string> &ip) {
  const auto padded = aoc::pad(ip);
  int ans = 0;
  for (size_t irow = 1; irow < padded.size() - 1; ++irow) {
    for (size_t icol = 1; icol < padded[irow].size() - 1; ++icol) {
      if (is_xmas(padded, irow, icol))
        ans++;
    }
  }
  println("ans part2 = {}", ans);
}

int main() {
  try {
    part1(test, "XMAS");
    part2(test);
    part1(aoc::read_file("input"), "XMAS");
    part2(aoc::read_file("input"));
  } catch (const std::exception &e) {
    println("{}", e.what());
  }
}

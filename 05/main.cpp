#include "../aoc.h"
#include <algorithm>
#include <array>
#include <functional>
#include <iterator>
#include <numeric>
#include <print>

using std::array, std::map, std::set, std::string, std::vector,
    std::back_inserter, std::ranges::is_sorted, std::ranges::sort, std::stoi,
    std::plus, std::println, std::transform_reduce, std::ranges::transform;

using Rules = map<int, set<int>>;

struct Pages {
  Rules rules;
  vector<vector<int>> nos;
};

Pages parse(const vector<vector<string>> &ip) {
  Pages pages;
  assert(ip.size() == 2);
  auto &rules = ip.at(0);
  for (const string &line : rules) {
    // println("{} {} {}",line,line.substr(0, 2),line.substr(3, 2));
    const int i1 = stoi(line.substr(0, 2));
    const int i2 = stoi(line.substr(3, 2));
    pages.rules[i1].insert(i2);
    pages.rules[i2]; // make sure all vertices have key in graph map
  }
  auto &nos = ip.at(1);
  for (const string &line : nos) {
    auto vs = aoc::split(line, ',');
    vector<int> vi;
    vi.reserve(vs.size());
    transform(vs, back_inserter(vi), [](const string &s) { return stoi(s); });
    pages.nos.push_back(vi);
  }
  return pages;
}

void all_that_follow_impl(int ipg, const map<int, set<int>> &graph,
                          set<int> &following) {
  for (int i : graph.at(ipg)) {
    if (!following.contains(i)) {
      following.insert(i);
      all_that_follow_impl(i, graph, following);
    }
  }
}

auto all_that_follow(const map<int, set<int>> &graph) {
  map<int, set<int>> after_graph = graph;
  for (auto &[i, adj] : graph) {
    set<int> following;
    all_that_follow_impl(i, graph, following);
    after_graph[i] = following;
  }
  return after_graph;
}

void day5(const string &fn) {
  const auto [graph, nos] = parse(aoc::read_paragraph_ip(fn));

  const auto after = all_that_follow(graph);

  auto cmp = [&](int x, int y) { return !graph.at(y).contains(x); };

  auto get12 = [&](const vector<int> &v) -> array<int, 2> {
    if (is_sorted(v, cmp)) {
      return {v.at(v.size() / 2), 0};
    } else {
      auto v2 = v;
      sort(v2, cmp);
      return {0, v2.at(v2.size() / 2)};
    }
  };

  auto add = [](const array<int, 2> &a, const array<int, 2> &b) {
    return array{a[0] + b[0], a[1] + b[1]};
  };

  auto [a1,a2] =
      transform_reduce(nos.begin(), nos.end(), array{0, 0}, add, get12);

  println("day 5 part 1 ans = {}", a1);
  println("day 5 part 2 ans = {}", a2);
}

int main() {
  try {
    day5("test_input");
    day5("input");
  } catch (const std::exception &e) {
    println("{}", e.what());
  }
}

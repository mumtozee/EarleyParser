#include <iostream>

#include "Solution.h"

int main() {
  CFG cfg{};  //   0    1    2    3    4    5    6
  cfg.symbols_ = {'$', 'S', 'T', 'U', 'a', 'b', 'c'};
  cfg.rules_ = {
      {0, {1}},        // S'->S
      {1, {4}},        // S->a
      {1, {6, 2}},     // S->cT
      {2, {6}},        // T -> c
      {2, {5, 3, 5}},  // T -> bUb
      {3, {4}},        // U -> a
      {3, {1, 5, 4}}   // U -> Sba
  };
  cfg.term_cnt = 3;
  cfg.nonterm_cnt = 4;

  Algo algo{cfg};

  std::string word = "cc";
  //  std::string word = "cbababc";
  auto res = algo.predict(word);
  std::cout << algo;
  std::cout << std::boolalpha << res << std::endl;
  return 0;
}

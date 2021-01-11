#include <gtest/gtest.h>

#include "Solution.h"

CFG CreateCFG() {
  CFG cfg{};  //    0    1    2    3    4    5    6
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
  return cfg;
}

Algo CreateParser() {
  auto cfg = CreateCFG();
  Algo algo{cfg};
  algo.configs_ = std::vector<Configuration>(4, Configuration{});
  Rule initial_rule{0, {1}};
  algo.configs_[0].emplace(initial_rule, 0, 0);
  return algo;
}

template <class T>
bool Equal(const std::set<T>& lhs, const std::set<T>& rhs) {
  for (const T& item : lhs) {
    if (rhs.find(item) == rhs.end()) {
      return false;
    }
  }
  return lhs.size() == rhs.size();
}

TEST(TestFunctionality, TestPredict) {
  std::set<State> correct_set = {
      {{0, {1}}, 0, 0},
      {{1, {4}}, 0, 0},
      {{1, {6, 2}}, 0, 0},
  };
  auto parser = CreateParser();
  parser.Predict(0);
  ASSERT_TRUE(Equal(parser.configs_[0], correct_set));
}

TEST(TestFunctionality, TestScan) {
  auto parser = CreateParser();
  parser.Predict(0);
  parser.Scan("cc", 0);
  State result{{1, {6, 2}}, 1, 0};
  ASSERT_TRUE(parser.configs_[1].find(result) != parser.configs_[1].end());
}

TEST(TestFunctionality, TestComplete) {
  auto parser = CreateParser();
  parser.Predict(0);
  parser.Scan("cc", 0);
  parser.Predict(1);
  parser.Complete(1);
  Configuration correct_set = {
      {{1, {6, 2}}, 1, 0}, {{2, {5, 3, 5}}, 0, 1}, {{2, {6}}, 0, 1}};
  ASSERT_TRUE(Equal(parser.configs_[1], correct_set));
}

TEST(TestFunctionality, TestParser) {
  auto cfg = CreateCFG();
  Algo parser{cfg};
  ASSERT_TRUE(parser.predict("cbab"));
  ASSERT_FALSE(parser.predict("cbaba"));
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

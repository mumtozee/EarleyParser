#ifndef SOLUTION_H
#define SOLUTION_H

#include <iostream>
#include <set>
#include <string>
#include <vector>

struct Rule {
  int lhs;
  std::vector<int> rhs;
};

bool operator<(const Rule& lhs, const Rule& rhs);
bool operator==(const Rule& lhs, const Rule& rhs);

struct State {
  Rule rule;
  int dot;
  int pos;
  State();
  State(Rule rule_, int dot_, int pos_);
};

bool operator<(const State& lhs, const State& rhs);
bool operator==(const State& lhs, const State& rhs);

struct CFG {
  std::set<Rule> rules_;
  std::vector<char> symbols_;
  int nonterm_cnt;
  int term_cnt;

  friend class Algo;
  bool IsTerminal(int idx) const;
};

using Configuration = std::set<State>;

class Algo {
 public:
  Algo();
  Algo(CFG cfg);
  bool predict(const std::string& word);
  std::vector<Configuration> configs_;
  CFG cfg_;
  void Predict(int j);
  void Scan(const std::string& word, int j);
  void Complete(int j);

  friend std::ostream& operator<<(std::ostream& out, const Algo& algo);
};

#endif  // SOLUTION_H

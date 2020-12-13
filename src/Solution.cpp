#include "Solution.h"

#include <iterator>
#include <utility>

static short LexSort(const std::vector<int>& lhs, const std::vector<int>& rhs) {
  size_t size = std::min(lhs.size(), rhs.size());
  for (size_t i = 0; i < size; ++i) {
    if (lhs[i] > rhs[i]) {
      return 1;
    } else if (lhs[i] < rhs[i]) {
      return -1;
    }
  }
  if (rhs.size() > lhs.size()) {
    return -1;
  }
  if (rhs.size() < lhs.size()) {
    return 1;
  }
  return 0;
}

bool operator<(const Rule& lhs, const Rule& rhs) {
  if (lhs.lhs == 0 && rhs.lhs > 0) {
    return true;
  }
  if (rhs.lhs == 0) {
    return false;
  }
  return (lhs.lhs < rhs.lhs) ||
         (lhs.lhs == rhs.lhs && LexSort(lhs.rhs, rhs.rhs) < 0);
}

bool operator==(const Rule& lhs, const Rule& rhs) {
  if (lhs.lhs != rhs.lhs) {
    return false;
  }
  if (lhs.rhs.size() != rhs.rhs.size()) {
    return false;
  }
  return LexSort(lhs.rhs, rhs.rhs) == 0;
}

State::State() = default;
State::State(Rule rule_, int dot_, int pos_)
    : rule(std::move(rule_)), dot(dot_), pos(pos_) {}

bool operator<(const State& lhs, const State& rhs) {
  return (lhs.rule < rhs.rule) || (lhs.rule == rhs.rule && lhs.dot < rhs.dot) ||
         (lhs.rule == rhs.rule && lhs.dot == rhs.dot && lhs.pos < rhs.pos);
}

bool operator==(const State& lhs, const State& rhs) {
  return (lhs.rule == rhs.rule && lhs.dot == rhs.dot && lhs.pos == rhs.pos);
}

bool CFG::IsTerminal(int idx) const { return idx >= nonterm_cnt; }

Algo::Algo() = default;

Algo::Algo(CFG cfg) : cfg_(std::move(cfg)) {}

void Algo::Predict(int j) {
  auto copy = configs_[j];
  for (const auto& state : configs_[j]) {
    if (state.dot < state.rule.rhs.size() &&
        !cfg_.IsTerminal(state.rule.rhs[state.dot])) {
      for (const auto& rule : cfg_.rules_) {
        if (rule.lhs == state.rule.rhs[state.dot]) {
          copy.emplace(rule, 0, j);
        }
      }
    }
  }
  configs_[j] = std::move(copy);
}

void Algo::Scan(const std::string& word, int j) {
  for (const auto& state : configs_[j]) {
    if (state.dot < state.rule.rhs.size() &&
        cfg_.IsTerminal(state.rule.rhs[state.dot])) {
      if (cfg_.symbols_[state.rule.rhs[state.dot]] == word[j]) {
        configs_[j + 1].emplace(state.rule, state.dot + 1, state.pos);
      }
    }
  }
}

void Algo::Complete(int j) {
  auto copy = configs_[j];
  for (const auto& state : configs_[j]) {
    if (state.dot == state.rule.rhs.size()) {
      for (const auto& state2 : configs_[state.pos]) {
        if (state2.dot < state2.rule.rhs.size() &&
            state2.rule.rhs[state2.dot] == state.rule.lhs) {
          copy.emplace(state2.rule, state2.dot + 1, state2.pos);
        }
      }
    }
  }
  configs_[j] = std::move(copy);
}

bool Algo::predict(const std::string& word) {
  configs_ = std::vector<Configuration>(word.size() + 1, Configuration{});
  Rule initial_rule{0, {1}};
  configs_[0].emplace(initial_rule, 0, 0);
  size_t old_size = configs_[0].size();
  size_t new_size = old_size;
  do {
    old_size = new_size;
    Predict(0);
    Complete(0);
    new_size = configs_[0].size();
  } while (old_size != new_size);
  for (int j = 1; j < configs_.size(); ++j) {
    Scan(word, j - 1);
    old_size = configs_[j].size();
    new_size = old_size;
    do {
      old_size = new_size;
      Predict(j);
      Complete(j);
      new_size = configs_[j].size();
    } while (old_size != new_size);
  }
  auto res_state = State{initial_rule, 1, 0};
  return configs_[word.size()].find(res_state) != configs_[word.size()].end();
}

std::ostream& operator<<(std::ostream& out, const Algo& algo) {
  int i = 0;
  for (const auto& config : algo.configs_) {
    std::cout << 'D' << i << ": {";
    for (const auto& state : config) {
      std::cout << '(' << algo.cfg_.symbols_[state.rule.lhs] << "->";
      for (size_t j = 0; j < state.rule.rhs.size(); ++j) {
        if (j == state.dot) {
          std::cout << '*';
        }
        std::cout << algo.cfg_.symbols_[state.rule.rhs[j]];
      }
      if (state.dot == state.rule.rhs.size()) {
        std::cout << '*';
      }
      std::cout << ", " << state.pos << "), ";
    }
    std::cout << "}\n";
    ++i;
  }
  return out;
}

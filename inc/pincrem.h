#pragma once
#include <algorithm>
#include <heuristicproblem.h>

class ProblemIncrem : public HeuristicProblem {
  size_t size;

public:
  ProblemIncrem(size_t size) : HeuristicProblem() { this->size = size; }
  tFitness fitness(const tSolution &solution) override;
  tSolution createSolution() override;
  size_t getSolutionSize() override { return size; }
  std::pair<tDomain, tDomain> getSolutionDomainRange() override {
    return std::make_pair(false, true);
  }

  vector<tHeuristic> heuristic(tSolution &sol, const vector<tOption> &options);
};

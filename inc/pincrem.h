#pragma once
#include <algorithm>
#include <heuristicproblem.h>

class ProblemIncrem : public HeuristicProblem {
  size_t size;

public:
  ProblemIncrem(size_t size) : HeuristicProblem() { this->size = size; }
  tFitness fitness(tSolution &solution) override;
  tSolution createSolution() override;
  size_t getSize() override { return size; }
  tHeuristic heuristic(tSolution &sol, tOption option);
};

#pragma once
#include <algorithm>
#include <problem.h>

class ProblemIncrem : public Problem {
  size_t size;

public:
  ProblemIncrem(size_t size) : Problem() { this->size = size; }
  tFitness fitness(tSolution &solution) override;
  tSolution createSolution() override;
  size_t getSize() override { return size; }
};

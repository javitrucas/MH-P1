#include <algorithm>
#include <pincrem.h>
#include <random.hpp>

tFitness ProblemIncrem::fitness(tSolution &solution) {
  tFitness count = 0;

  for (int i = 0; i < solution.size(); i++) {
    if (solution[i]) {
      if (i % 2 == 0) {
        count += 1;
      } else {
        count -= 1;
      }
    }
  }
  return count;
}

tSolution ProblemIncrem::createSolution() {
  tSolution solution(size);
  for (int i = 0; i < solution.size(); i++) {
    solution[i] = Random::get<bool>();
  }
  return solution;
}

tHeuristic ProblemIncrem::heuristic(tSolution &sol, tOption option) {
  tHeuristic value = ((option % 2) == 1 ? sol.size() - option : sol.size());
  return value;
}

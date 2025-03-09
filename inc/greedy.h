#pragma once
#include <heuristicmh.h>

using namespace std;

/**
 * Implementation of the Random Search metaheuristic
 *  - Randomly generates solutions and selects the best one
 *
 * @see MH
 * @see Problem
 */
class GreedySearch : public HeuristicMH {

public:
  GreedySearch() : HeuristicMH() {}
  virtual ~GreedySearch() {}
  // Implement the MH interface methods
  /**
   * Create random solutions until maxevals has been achieved, and returns the
   * best one.
   *
   * @param problem The problem to be optimized
   * @param maxevals Maximum number of evaluations allowed
   * @return A pair containing the best solution found and its fitness
   */
  ResultMH optimize(HeuristicProblem *problem, int maxevals) override;
};

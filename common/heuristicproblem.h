#ifndef __HEURISTICPROBLEM_H
#define __HEURISTICPROBLEM_H

#include "problem.h"

/**
 * Problem class to solve the given problem in metaheuristic.
 *
 * @author your_name
 * @version 1.0
 */
class HeuristicProblem : public Problem {
public:
  /**
   * This is heuristic for the problem
   */
  virtual tHeuristic heuristic(tSolution &current_sol, tOption new_option) = 0;
};

#endif

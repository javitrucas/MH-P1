#ifndef __HEURISTICPROBLEM_H
#define __HEURISTICPROBLEM_H

#include "problem.h"
#include <vector>

using std::vector;

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
  virtual vector<tHeuristic> heuristic(tSolution &current_sol,
                                       const vector<tOption> &new_option) = 0;
};

#endif

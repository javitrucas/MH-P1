#pragma once

#include "mh.h"
#include "solution.h"
#include <heuristicproblem.h>
#include <utility>

using std::pair;

/**
 * Metaheuristic algorithm interface.
 *
 * @author  dmolina
 * @version 1.0
 * @since   2021-05-01
 */
class HeuristicMH : public MH {
public:
  virtual ~HeuristicMH() {}
  /**
   * Run the metaheuristic algorithm to find the optimal solution.
   *
   * @param problem  The problem to solve.
   * @param maxevals The maximum number of evaluations.
   * @return A pair containing the optimal solution and its fitness.
   * @throws Exception if the algorithm fails to converge.
   *
   * @since   2021-05-01
   * @author  author
   * @version 1.0
   */
  virtual ResultMH optimize(HeuristicProblem *problem, int maxevals) = 0;
  ResultMH optimize(Problem *problem, int maxevals) override {
    return optimize((HeuristicProblem *)problem, maxevals);
  }
};

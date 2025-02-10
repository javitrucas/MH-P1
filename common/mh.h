#pragma once

#include <problem.h>
#include <utility>

using std::pair;

/**
 * Metaheuristic algorithm interface.
 *
 * @author  author
 * @version 1.0
 * @since   2021-05-01
 */
class MH {
public:
  virtual ~MH() {}
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
  virtual pair<tSolution, tFitness> optimize(Problem *problem,
                                             int maxevals) = 0;
};

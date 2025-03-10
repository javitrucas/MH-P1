#ifndef __PROBLEM_H
#define __PROBLEM_H

#include "solution.h"
#include <random.hpp>
#include <utility>

// get base random alias which is auto seeded and has static API and internal
// state it is not threads secure, you can also use ::random_thread_local
using Random = effolkronium::random_static;

/**
 * Problem class to solve the given problem in metaheuristic.
 *
 * @author your_name
 * @version 1.0
 */
class Problem {
public:
  Problem() {}
  /***
   * Evaluate the solution from scratch.
   * @param solution to evaluate.
   */
  virtual tFitness fitness(const tSolution &solution) = 0;
  /**
   * Evaluate the solution indicating the current position to change and the new
   * value.
   *
   * By default it run the previous one. However, it can be override to
   * factorize the fitness.
   *
   * @param solution to evaluate.
   * @param pos_previous position of the solution to change.
   * @param new_value.
   */
  virtual tFitness fitness(const tSolution &solution, unsigned pos_previous,
                           tDomain new_value) {
    tSolution newsol(solution);
    newsol[pos_previous] = new_value;
    return fitness(newsol);
  }
  /**
   * Create a new solution.
   */
  virtual tSolution createSolution() = 0;
  /**
   * Return the current size of the solution.
   */
  virtual size_t getSolutionSize() = 0; // Get the size of each solution
  /** Return the range of domain of each element of the solution */
  virtual std::pair<tDomain, tDomain> getSolutionDomainRange() = 0;
};

#endif

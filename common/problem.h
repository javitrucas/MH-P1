#ifndef __PROBLEM_H
#define __PROBLEM_H

#include "solution.h"
#include <random.hpp>

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
  virtual tFitness fitness(tSolution &solution) = 0;
  virtual tSolution createSolution() = 0;
  virtual size_t getSize() = 0;
};

#endif

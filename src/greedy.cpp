#include <cassert>
#include <greedy.h>
#include <iostream>

using namespace std;

template <class T> void print_vector(string name, const vector<T> &sol) {
  cout << name << ": ";

  for (auto elem : sol) {
    cout << elem << ", ";
  }
  cout << endl;
}

/**
 * Create random solutions until maxevals has been achieved, and returns the
 * best one.
 *
 * @param problem The problem to be optimized
 * @param maxevals Maximum number of evaluations allowed
 * @return A pair containing the best solution found and its fitness
 */
ResultMH GreedySearch::optimize(HeuristicProblem *problem, int maxevals) {
  assert(maxevals > 0);
  vector<uint> values;
  tSolution sol(problem->getSize());
  print_vector("sol_initial", sol);

  auto size = problem->getSize();

  for (int i = 0; i < size; i++) {
    values.push_back(i);
  }

  for (int r = 0; r < size / 2; r++) {
    vector<float> heuristics;

    for (uint posi : values) {
      float heur = problem->heuristic(sol, posi);
      heuristics.push_back(heur);
    }

    // print_vector("heuristics", heuristics);

    auto posi = min_element(heuristics.begin(), heuristics.end());
    int posi_int = posi - heuristics.begin();

    sol[values[posi_int]] = 1;
    values[posi_int] = values.back();
    values.pop_back();
  }

  tFitness fitness = problem->fitness(sol);
  return ResultMH(sol, fitness, 1);
}

#include <cassert>
#include <greedy.h>
#include <iostream>
#include <pincrem.h>

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
ResultMH GreedySearch::optimize(Problem *problem, int maxevals) {
  assert(maxevals > 0);

  size_t size = problem->getSolutionSize();
  ProblemIncrem *realproblem = dynamic_cast<ProblemIncrem *>(problem);
  int m = realproblem->getM();

  // Vector para almacenar la solución final
  tSolution sol;
  
  vector<tOption> values;
  // Crear vector de valores no seleccionados
  for (int i = 0; i < size; i++) {
    values.push_back(i);
  }

  // Elegir el primer elemento aleatorio
  int first = Random::get<int>(0, size - 1);
  sol.push_back(first);
  cout << "Primer candidato: " << first << endl;
  // Eliminar el primer elemento de los valores no seleccionados
  values.erase(remove(values.begin(), values.end(), first), values.end());
  
  print_vector("Values", values);

  // Seleccionar los m-1 elementos restantes
  for (size_t i = 1; i < m; i++) {
    int best_candidate = -1;
    tFitness best_fitness = std::numeric_limits<tFitness>::max();
    // Evaluar todos los candidatos no seleccionados
    for (int candidate : values) {
      // Crear una nueva solución con el candidato
      tSolution new_sol = sol;
      new_sol.push_back(candidate);
      print_vector("New solution", new_sol);
      // Evaluar la nueva solución
      tFitness fitness = problem->fitness(new_sol);
      // Si la nueva solución es mejor que la mejor encontrada hasta ahora
      if (fitness < best_fitness) {
        best_candidate = candidate;
        best_fitness = fitness;
      }
    }
    // Si se ha encontrado un candidato, añadirlo a la solución y eliminarlo de los valores no seleccionados
    if(best_candidate != -1 && !values.empty()) {
      sol.push_back(best_candidate);
      values.erase(remove(values.begin(), values.end(), best_candidate), values.end());
    }
  }

  tFitness fitness = problem->fitness(sol);
  return ResultMH(sol, fitness, 1);
}

#include <cassert>
#include <localsearch.h>
#include <util.h>
#include <pincrem.h>
#include <set>


using namespace std;

ResultMH LocalSearch::optimize(Problem *problem, int maxevals) {
  assert(maxevals > 0);

  const int MAX_EVALS_LS = 100000; // Máximo de evaluaciones para la búsqueda local

  size_t size = problem->getSolutionSize();
  ProblemIncrem *realproblem = dynamic_cast<ProblemIncrem *>(problem);
  int m = realproblem->getM();

  // Vector para almacenar la solución final
  tSolution best;
  tFitness best_fitness = -1;

  // 1. Generar una solución inicial aleatoria
  tSolution solution = problem->createSolution();
  tFitness fitness = problem->fitness(solution);
  int evals = 1;

  // 2. Generar la inf factorizada
  SolutionFactoringInfo *info = problem->generateFactoringInfo(solution);
  
  // 3. Definir conjunto de elementos seleccionados y no seleccionados
  std::set<int> selected(solution.begin(), solution.end());
  std::set<int> not_selected;
  for (int i = 0; i < size; i++) {
    if (selected.find(i) == selected.end()) {
      not_selected.insert(i);
    }
  }

  bool mejorado = true;
  while(mejorado && evals<MAX_EVALS_LS) {
    mejorado = false;

    // 4. Generar lista de intercambios posibles
    std::vector<std::pair<int, int>> swaps;
    for (int sel : selected) {
      for (int non_sel : not_selected) {
          swaps.push_back(std::make_pair(sel, non_sel));
      }
    }

    if (swaps.empty()) {
      std::cerr << "No hay movimientos posibles" << std::endl;
      break;
    }

    // 5. Ordenar movimientos segun estrategia
    if(estrategia == EstrategiaExploracion::heurLS) {
      std::sort(swaps.begin(), swaps.end(), [&](const std::pair<int, int> &a, const std::pair<int, int> &b) {
        auto it_a = std::find(solution.begin(), solution.end(), a.first);
        auto it_b = std::find(solution.begin(), solution.end(), b.first);
        // Evaluar la contribución de cada movimiento
        tFitness deltaA = problem->fitness(solution, info, 
          std::distance(solution.begin(), it_a), a.second);
        tFitness deltaB = problem->fitness(solution, info, 
          std::distance(solution.begin(), it_b), b.second);
        return deltaA < deltaB; // Priorizar menor dispersión
      });

    } else {
      Random::shuffle(swaps.begin(), swaps.end());
    }

    // 6. Aplicar el primer movimiento que mejore la solucion
    for (size_t i = 0; i < swaps.size(); i++) {
      int sel = swaps[i].first;
      int non_sel = swaps[i].second;

      auto it_sel = std::find(solution.begin(), solution.end(), sel);
      if (it_sel == solution.end()) {
          std::cerr << "Error: Elemento seleccionado no encontrado" << std::endl;
          continue;
      }
      tFitness new_fitness = problem->fitness(solution, info, std::distance(solution.begin(), it_sel), non_sel);
      evals++;

      if (new_fitness < fitness) {  // Si encontramos mejora
          // Aplicar movimiento
          selected.erase(sel);
          selected.insert(non_sel);
          std::replace(solution.begin(), solution.end(), sel, non_sel);
          not_selected.erase(non_sel);
          not_selected.insert(sel);

          // Actualizar información factorizada
          int pos_change = std::distance(solution.begin(), std::find(solution.begin(), solution.end(), non_sel));
          problem->updateSolutionFactoringInfo(info, solution, pos_change, non_sel);

          fitness = new_fitness;
          mejorado = true;
          break; // Primer mejor encontrado
      }

      if (evals >= MAX_EVALS_LS) break;
    }  
    
  }

  delete info; // Liberar memoria de la info factorizada

  return ResultMH(solution, fitness, evals);

}
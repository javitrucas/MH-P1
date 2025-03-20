#pragma once
#include <algorithm>
#include <heuristicproblem.h>

class ProblemIncrem : public HeuristicProblem {
  size_t size;                // Número de elementos (n)

private:
  int m;                        // Número de elementos seleccionados (m)
  std::vector<std::vector<double>> D;  // Matriz de distancias

public:
  ProblemIncrem(size_t size) : HeuristicProblem() { this->size = size; }
  ProblemIncrem() : HeuristicProblem() {size=0; m=0;}
  tFitness fitness(const tSolution &solution) override;
  tSolution createSolution() override;
  size_t getSolutionSize() override { return size; }
  std::pair<tDomain, tDomain> getSolutionDomainRange() override {
    return std::make_pair(false, true);
  }

  vector<tHeuristic> heuristic(tSolution &sol, const vector<tOption> &options);

  void leerArchivo(const std::string& filename);
};

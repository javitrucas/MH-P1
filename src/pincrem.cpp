#include <algorithm>
#include <pincrem.h>
#include <random.hpp>
#include <fstream>
#include <iostream>

tFitness ProblemIncrem::fitness(const tSolution &solution) {
  tFitness count = 0;

  for (int i = 0; i < solution.size(); i++) {
    if (solution[i]) {
      if (i % 2 == 0) {
        count += 1;
      } else {
        count -= 1;
      }
    }
  }
  return count;
}

tSolution ProblemIncrem::createSolution() {
  tSolution solution(size);
  for (int i = 0; i < solution.size(); i++) {
    solution[i] = Random::get<bool>();          // Numero aleatorio entre 1 y n
  }
  return solution;
}

vector<tHeuristic> ProblemIncrem::heuristic(tSolution &sol,
                                            const vector<tOption> &options) {
  vector<tHeuristic> values;

  for (auto option : options) {
    tHeuristic value = ((option % 2) == 1 ? sol.size() - option : sol.size());
    values.push_back(value);
  }

  return values;
}

void ProblemIncrem::leerArchivo(const std::string& filename){
  std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: No se pudo abrir el archivo " << filename << std::endl;
        exit(1);
    }

    file >> n >> m;
    D.resize(n, std::vector<double>(n, 0.0));

    int i, j;
    double dij;
    while (file >> i >> j >> dij) {
        D[i][j] = dij;
        std::cout << D[i][j] << std::endl;
        //D[j][i] = dij;  // Matriz simétrica
    }

    file.close();
    std::cout << "archivo " <<  filename << " leido correctamente." << std::endl;
}

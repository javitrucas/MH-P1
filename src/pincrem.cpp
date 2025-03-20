#include <algorithm>
#include <pincrem.h>
#include <random.hpp>
#include <fstream>
#include <iostream>

tFitness ProblemIncrem::fitness(const tSolution &solution) {
  tFitness count_parcial[m];
  tFitness count_total=0;
  int max = 99999999;
  int min = -1;

  for (int i = 0; i < m; i++) {
    for (int j = 0; j < m; j++){
      if(j!=i) {
        count_parcial[i] += D[i][j];
      }
    }
    if (count_parcial[i]<min){
      min = count_parcial[i];
    }
    if(count_parcial[i]>max){
      max = count_parcial[i];
    }
  }
  count_total = max - min;
  return count_total;
}

tSolution ProblemIncrem::createSolution() {
  tSolution solution(size);
  for (int i = 0; i < solution.size(); i++) {
    solution[i] = Random::get<int>(1,size);          // Numero aleatorio entre 1 y n
  }             // COMPROBAR QUE NO SE REPITAN NUMEROS
  // Ordenar numeros de menor a mayor
  std::sort(solution.begin(), solution.end());
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

    file >> size >> m;
    D.resize(size, std::vector<double>(size, 0.0));

    int i, j;
    double dij;
    while (file >> i >> j >> dij) {
        D[i][j] = dij;
        //std::cout << D[i][j] << std::endl;
        //D[j][i] = dij;  // Matriz simétrica
    }

    file.close();
    std::cout << "archivo " <<  filename << " leido correctamente." << std::endl;
}

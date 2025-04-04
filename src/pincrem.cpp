#include <algorithm>
#include <pincrem.h>
#include <random.hpp>
#include <fstream>
#include <iostream>
#include <set>

// Calcula la diferencia entre el nodo más conectado y el menos conectado
tFitness ProblemIncrem::fitness(const tSolution &solution) {
  std::vector<tFitness> count_parcial(solution.size(), 0);
  tFitness count_total = 0.0f;
  float max = std::numeric_limits<int>::min();
  float min = std::numeric_limits<int>::max();

  for (int i = 0; i < solution.size(); i++) {
    for (int j = 0; j < solution.size(); j++) {
      if (j != i) {
        int index1 = solution[i];
        int index2 = solution[j];

        // Asegurarse de acceder a la matriz de forma válida
        if (index1 >= 0 && index2 >= 0 && index1 < size && index2 < size) {
          if (index1 > index2) std::swap(index1, index2);
          count_parcial[i] += D[index1][index2];  // Acumulamos la conexión
        } else {
          std::cerr << "Error: índices fuera de rango en fitness(): " << index1 << ", " << index2 << std::endl;
        }
      }
    }
    // Actualización de máximo y mínimo
    if (count_parcial[i] < min) min = count_parcial[i];
    if (count_parcial[i] > max) max = count_parcial[i];
  }

  // Fitness es la diferencia entre el más y el menos conectado
  count_total = max - min;
  return count_total;
}

// Genera una solución aleatoria válida con m elementos únicos
tSolution ProblemIncrem::createSolution() {
  std::set<int> unique_values;

  // Generar m valores únicos aleatorios entre 0 y size-1
  while (unique_values.size() < m) {
    int random_value = Random::get<int>(0, size - 1);
    unique_values.insert(random_value);
  }

  // Convertir el set a vector y ordenar
  tSolution solution(unique_values.begin(), unique_values.end());
  std::sort(solution.begin(), solution.end());

  return solution;
}

// Lee el archivo con la matriz de distancias o pesos
void ProblemIncrem::leerArchivo(const std::string& filename) {
  std::ifstream file(filename);
  if (!file) {
    std::cerr << "Error: No se pudo abrir el archivo " << filename << std::endl;
    exit(1);
  }

  file >> size >> m;
  std::cout << "Size(n): " << size << std::endl;
  std::cout << "m:" << m << std::endl;
  D.resize(size, std::vector<float>(size, 0.0));

  int i, j;
  double dij;
  while (file >> i >> j >> dij) {
    D[i][j] = dij;
  }

  file.close();
  std::cout << "archivo " << filename << " leido correctamente." << std::endl;
}

// Calcula la información de factorización para cada nodo de la solución
SolutionFactoringInfo *ProblemIncrem::generateFactoringInfo(const tSolution &solution) {
  ProblemIncremFactoringInfo *info = new ProblemIncremFactoringInfo(m);

  for (size_t i = 0; i < m; i++) {
    for (size_t j = 0; j < m; j++) {
      if (i != j) {
        int index1 = solution[i];
        int index2 = solution[j];

        if (index1 >= 0 && index2 >= 0 && index1 < size && index2 < size) {
          if (index1 > index2) std::swap(index1, index2);
          info->fitness_parcial[i] += D[index1][index2];
        } else {
          std::cerr << "Error: índices fuera de rango en generateFactoringInfo()" << std::endl;
        }
      }
    }
  }
  return info;
}

// Actualiza solo la parte afectada del fitness tras un cambio en la solución
void ProblemIncrem::updateSolutionFactoringInfo(SolutionFactoringInfo *solution_info,
                                                const tSolution &solution,
                                                unsigned pos_change,
                                                tDomain new_value) {
  ProblemIncremFactoringInfo *info = static_cast<ProblemIncremFactoringInfo*>(solution_info);
  int old_value = solution[pos_change];

  for (int i = 0; i < m; i++) {
    if (i != pos_change) {
      int index1 = solution[i];
      int index2 = old_value;
      if (index1 > index2) std::swap(index1, index2);
      info->fitness_parcial[pos_change] -= D[index1][index2];

      index2 = new_value;
      if (index1 > index2) std::swap(index1, index2);
      info->fitness_parcial[pos_change] += D[index1][index2];
    }
  }
}

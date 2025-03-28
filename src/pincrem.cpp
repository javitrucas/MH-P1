#include <algorithm>
#include <pincrem.h>
#include <random.hpp>
#include <fstream>
#include <iostream>
#include <set>

tFitness ProblemIncrem::fitness(const tSolution &solution) {
  
  //std::cout<<"m= " << solution.size() << std::endl;
  
  // Caso especial: m = 2
  /*if (solution.size() == 2) {
    int index1 = solution[0];
    int index2 = solution[1];

    // Verificar que los índices sean válidos
    if (index1 >= 0 && index2 >= 0 && index1 < size && index2 < size) {
        if (index1 > index2) std::swap(index1, index2); // Asegurar acceso correcto a D
        tFitness result = D[index1][index2];
        std::cout << "Fitness para m=2: " << result << std::endl;
        return result;
    } else {
        std::cerr << "Error: índices fuera de rango en fitness() para m=2: " 
                  << index1 << ", " << index2 << std::endl;
        return std::numeric_limits<tFitness>::max(); // Retornar un valor grande como error
    }
  }*/
  
  //std::cout << "Dentro de fitness" << std::endl;
  std::vector<tFitness> count_parcial(solution.size(), 0);  // Usar vector dinámico
  tFitness count_total=0.0f;
  //int max = -1;
  //int min = 99999999;
  float max = std::numeric_limits<int>::min();
  float min = std::numeric_limits<int>::max();

  for (int i = 0; i < solution.size(); i++) {
    for (int j = 0; j < solution.size(); j++){
      if(j!=i) {
        int index1 = solution[i];
        int index2 = solution[j];
        // Verificar que los índices sean válidos
        if (index1 >= 0 && index2 >= 0 && index1 < size && index2 < size) {
            if (index1 > index2) std::swap(index1, index2); // Para acceder a D correctamente
            count_parcial[i] += D[index1][index2];
        } else {
            std::cerr << "Error: índices fuera de rango en fitness(): " << index1 << ", " << index2  << std::endl;
        }
        //std::cout << "Matriz " << index1 << ", " << index2 << ": " << D[index1][index2] << std::endl;
      }
    }
    if (count_parcial[i]<min){
      //std::cout << "Nuevo min: " << count_parcial[i] << std::endl;
      min = count_parcial[i];
    }
    if(count_parcial[i]>max){
      //std::cout << "Nuevo max: " << count_parcial[i] << std::endl;
      max = count_parcial[i];
    }
  }
  count_total = max - min;
  //std::cout << "Count_total: " << count_total << std::endl;
  return count_total;
}

tSolution ProblemIncrem::createSolution() {
  std::set<int> unique_values;

  // Generar m valores únicos aleatorios
  while (unique_values.size() < m) {
      int random_value = Random::get<int>(0, size - 1); // Valores entre 0 y n-1
      unique_values.insert(random_value); // Insertar en el set (evita duplicados)
  }

  // Convertir el set en un vector
  tSolution solution(unique_values.begin(), unique_values.end());

  // Ordenar la solución de menor a mayor
  std::sort(solution.begin(), solution.end());

  return solution;
}

/*vector<tHeuristic> ProblemIncrem::heuristic(tSolution &sol,
                                            const vector<tOption> &options) {
  vector<tHeuristic> values;

  for (auto option : options) {
    tHeuristic value = ((option % 2) == 1 ? sol.size() - option : sol.size());
    values.push_back(value);
  }

  return values;
}*/

void ProblemIncrem::leerArchivo(const std::string& filename){
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
        //std::cout << D[i][j] << std::endl;
        //D[j][i] = dij;  // Matriz simétrica
    }

    file.close();
    std::cout << "archivo " <<  filename << " leido correctamente." << std::endl;
}

SolutionFactoringInfo *ProblemIncrem::generateFactoringInfo(const tSolution &solution) {
  ProblemIncremFactoringInfo *info = new ProblemIncremFactoringInfo(m);
  // Si size_t da problemas, cambiar a int
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

void ProblemIncrem::updateSolutionFactoringInfo(SolutionFactoringInfo *solution_info,
                                                const tSolution &solution,
                                                unsigned pos_change,
                                                tDomain new_value) {
  ProblemIncremFactoringInfo *info = static_cast<ProblemIncremFactoringInfo*>(solution_info);
  // Valor anterior en la posición cambiada
  int old_value = solution[pos_change];
  // Actualizar el fitness parcial para la posición modificada
  for (int i = 0; i < m; i++) {
    if (i != pos_change) {
      int index1 = solution[i];
      int index2 = old_value;
      // Asegurar que accedemos a la parte superior de la matriz simétrica
      if (index1 > index2) std::swap(index1, index2);
      info->fitness_parcial[pos_change] -= D[index1][index2];

      index2 = new_value;
      if (index1 > index2) std::swap(index1, index2);
      info->fitness_parcial[pos_change] += D[index1][index2];
    }
  }
  //solution[pos_change] = new_value;
}

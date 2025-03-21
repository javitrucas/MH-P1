#include <algorithm>
#include <pincrem.h>
#include <random.hpp>
#include <fstream>
#include <iostream>

tFitness ProblemIncrem::fitness(const tSolution &solution) {
  std::cout << "Dentro de fitness" << std::endl;
  std::vector<tFitness> count_parcial(m, 0);  // Usar vector dinámico
  tFitness count_total=0;
  //int max = -1;
  //int min = 99999999;
  int max = std::numeric_limits<int>::min();
  int min = std::numeric_limits<int>::max();

  for (int i = 0; i < m; i++) {
    for (int j = 0; j < m; j++){
      if(j!=i) {
        int index1 = solution[i];
                int index2 = solution[j];
                
                // Verificar que los índices sean válidos
                if (index1 >= 0 && index2 >= 0 && index1 < size && index2 < size) {
                    if (index1 > index2) std::swap(index1, index2); // Para acceder a D correctamente
                    count_parcial[i] += D[index1][index2];
                } else {
                    std::cerr << "Error: índices fuera de rango en fitness()" << std::endl;
                }
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
  std::cout << "Count_total: " << count_total << std::endl;
  return count_total;
}

tSolution ProblemIncrem::createSolution() {
  //std::cout << "Dentro de create solution" << std::endl;
  tSolution solution(m);
  //std::cout << "Dentro de crear tsolution" << std::endl;
  for (int i = 0; i < m; i++) {         // Generar m elementos
    //std::cout << "Generando" << i << std::endl;
    solution[i] = Random::get<int>(0,size-1);          // Numero aleatorio entre 1 y n
    //std::cout << "Elemento escogido: " << solution[i] << " de " << solution.size() << std::endl;
  }             // COMPROBAR QUE NO SE REPITAN NUMEROS
  // Ordenar numeros de menor a mayor
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

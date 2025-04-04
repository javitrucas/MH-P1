#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include "greedy.h"
#include "pincrem.h"
#include "random.hpp"

using namespace std;

/**
 * Imprime un vector en la consola.
 *
 * @tparam T Tipo de los elementos del vector.
 * @param label Etiqueta para identificar el vector.
 * @param vec Vector a imprimir.
 */
template <typename T>
void printVector(const string& label, const vector<T>& vec) {
    cout << label << ": ";
    for (const auto& elem : vec) {
        cout << elem << " ";
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
ResultMH GreedySearch::optimize(Problem* problem, int maxEvals) {
    assert(maxEvals > 0);

    // Obtener el tamaño de la solución y el número de elementos a seleccionar
    size_t solutionSize = problem->getSolutionSize();
    ProblemIncrem* realProblem = dynamic_cast<ProblemIncrem*>(problem);
    int elementsToSelect = realProblem->getM();

    // Inicializar la solución y los candidatos disponibles
    tSolution currentSolution;
    vector<tOption> availableCandidates;
    for (int i = 0; i < solutionSize; ++i) {
        availableCandidates.push_back(i);
    }

    // Seleccionar el primer candidato aleatorio
    int firstCandidate = Random::get<int>(0, static_cast<int>(solutionSize) - 1);
    currentSolution.push_back(firstCandidate);
    availableCandidates.erase(remove(availableCandidates.begin(), availableCandidates.end(), firstCandidate), availableCandidates.end());

    cout << "Primer candidato seleccionado: " << firstCandidate << endl;
    printVector("Candidatos restantes", availableCandidates);

    // Seleccionar los elementos restantes usando una estrategia voraz
    for (int i = 1; i < elementsToSelect; ++i) {
        int bestCandidate = -1;
        tFitness bestFitness = numeric_limits<tFitness>::max();

        for (const auto& candidate : availableCandidates) {
            // Crear una nueva solución temporal con el candidato actual
            tSolution tempSolution = currentSolution;
            tempSolution.push_back(candidate);

            // Evaluar la solución temporal
            tFitness candidateFitness = problem->fitness(tempSolution);

            // Actualizar el mejor candidato si es necesario
            if (candidateFitness < bestFitness) {
                bestCandidate = candidate;
                bestFitness = candidateFitness;
            }
        }

        // Añadir el mejor candidato a la solución y eliminarlo de los disponibles
        if (bestCandidate != -1) {
            currentSolution.push_back(bestCandidate);
            availableCandidates.erase(remove(availableCandidates.begin(), availableCandidates.end(), bestCandidate), availableCandidates.end());
        }

        cout << "Candidato añadido: " << bestCandidate << endl;
        printVector("Nueva solución", currentSolution);
        printVector("Candidatos restantes", availableCandidates);
    }

    // Calcular el fitness final de la solución completa
    tFitness finalFitness = problem->fitness(currentSolution);
    
    return ResultMH(currentSolution, finalFitness, 1);
}
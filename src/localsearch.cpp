#include "localsearch.h"
#include "pincrem.h"
#include <algorithm>
#include <unordered_set>

using namespace std;

ResultMH LocalSearch::optimize(Problem* problem, int maxevals) {
    assert(maxevals > 0);

    const int MAX_EVAL_LIMIT = 100000;
    ProblemIncrem* realProblem = dynamic_cast<ProblemIncrem*>(problem);
    int m = realProblem->getM();
    size_t size = problem->getSolutionSize();

    tSolution currentSolution = problem->createSolution();
    tFitness currentFitness = problem->fitness(currentSolution);
    auto info = problem->generateFactoringInfo(currentSolution);
    
    unordered_set<int> selected(currentSolution.begin(), currentSolution.end());
    unordered_set<int> notSelected;
    for (int i = 0; i < size; ++i) {
        if (!selected.count(i))
            notSelected.insert(i);
    }

    bool improved = true;
    int evals = 1;

    while (improved && evals < MAX_EVAL_LIMIT) {
        improved = false;
        vector<pair<int, int>> swaps;

        // Generar todos los posibles swaps entre elementos seleccionados y no seleccionados
        for (int sel : selected) {
            for (int nonSel : notSelected) {
                swaps.emplace_back(sel, nonSel);
            }
        }

        if (swaps.empty()) break;

        // Para heurLS: buscar la mejor mejora (swap que minimice el fitness)
        if (explorationMode == SearchStrategy::heurLS) {
            tFitness bestCandidateFitness = currentFitness;
            pair<int, int> bestSwap = {-1, -1};
            int bestPos = -1;
            // Recorrer todos los swaps disponibles
            for (const auto& [sel, nonSel] : swaps) {
                auto posIt = find(currentSolution.begin(), currentSolution.end(), sel);
                if (posIt == currentSolution.end()) continue;
                int pos = posIt - currentSolution.begin();
                tFitness candidateFitness = problem->fitness(currentSolution, info, pos, nonSel);
                evals++;
                if (candidateFitness < bestCandidateFitness) {
                    bestCandidateFitness = candidateFitness;
                    bestSwap = {sel, nonSel};
                    bestPos = pos;
                }
                if (evals >= MAX_EVAL_LIMIT) break;
            }
            // Si se encontró un swap que mejora la solución, se aplica
            if (bestSwap.first != -1 && bestCandidateFitness < currentFitness) {
                int sel = bestSwap.first;
                int nonSel = bestSwap.second;
                selected.erase(sel);
                selected.insert(nonSel);
                replace(currentSolution.begin(), currentSolution.end(), sel, nonSel);
                notSelected.erase(nonSel);
                notSelected.insert(sel);
                problem->updateSolutionFactoringInfo(info, currentSolution, bestPos, nonSel);
                currentFitness = bestCandidateFitness;
                improved = true;
            }
        }
        else {
            Random::shuffle(swaps.begin(), swaps.end());
            for (const auto& [sel, nonSel] : swaps) {
                auto pos = find(currentSolution.begin(), currentSolution.end(), sel);
                if (pos == currentSolution.end()) continue;

                tFitness newFitness = problem->fitness(currentSolution, info, pos - currentSolution.begin(), nonSel);
                evals++;

                if (newFitness < currentFitness) {
                    selected.erase(sel);
                    selected.insert(nonSel);
                    replace(currentSolution.begin(), currentSolution.end(), sel, nonSel);
                    notSelected.erase(nonSel);
                    notSelected.insert(sel);

                    int newPos = find(currentSolution.begin(), currentSolution.end(), nonSel) - currentSolution.begin();
                    problem->updateSolutionFactoringInfo(info, currentSolution, newPos, nonSel);

                    currentFitness = newFitness;
                    improved = true;
                    break;
                }

                if (evals >= MAX_EVAL_LIMIT) break;
            }
        }
    }

    delete info;
    return ResultMH(currentSolution, currentFitness, evals);
}

#pragma once
#include <mh.h>

enum class EstrategiaExploracion {
  randLS,  // Exploración en orden aleatorio 
  heurLS // Exploración basada en heurística
};

class LocalSearch : public MH {
private:
  EstrategiaExploracion estrategia;

public:
  ResultMH optimize(Problem *problem, const int maxevals) override;
};
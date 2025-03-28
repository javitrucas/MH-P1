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
  LocalSearch( EstrategiaExploracion estrategia) : MH() { this->estrategia = estrategia; }
  virtual ~LocalSearch() {}
  ResultMH optimize(Problem *problem, const int maxevals) override;
};
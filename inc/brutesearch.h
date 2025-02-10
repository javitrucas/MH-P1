#include <mh.h>

class BruteSearch : public MH {
public:
  pair<tSolution, tFitness> optimize(Problem *problem,
                                     const int maxevals) override;
};

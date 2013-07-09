#ifndef MCAR_QLEARN_HPP
#define MCAR_QLEARN_HPP

#include "sml/QLearning.hpp"
#include "simu/MCar.hpp"

using namespace simu;
using namespace sml;

class MCarQLearn
{
  
public: 
    void mcar_qltable_learner();
    
    void mcar_qltable_teacher(float cost);
    
    void mcar_qltable_teacher_annonce(float cost);
    
private:
    
//private:
public:
    static const StateTemplate t_stempl;
};

#endif

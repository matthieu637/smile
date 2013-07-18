#ifndef POLICY_HPP
#define POLICY_HPP

#include "bib/Logger.hpp"
#include "Action.hpp"

namespace sml {

template<class State>
class Policy
{
  
public:
  virtual ~Policy(){}
  virtual DAction* decision(const State& st, float epsilon=0.L) = 0;
  virtual void clear_history(const State& s, const DAction& a) = 0;
};

typedef Policy<DState> DPolicy;

}




#endif // MCARTASK_H

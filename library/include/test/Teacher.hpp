#ifndef TEACHER_HPP
#define TEACHER_HPP

#include "simu/Environnement.hpp"
#include "simu/MCar.hpp"
#include <sml/Q.hpp>

#define FDB "feedbacks"

using namespace simu;

namespace test {
  
struct TeacherState{
    bool feedback;
    MCarState car;
    int action;
};

class Teacher : public Environnement<TeacherState> {
  
public:
    Teacher(MCar* prob);

    double reward() const;
    DAction* getInitialAction() const;
    bool goal() const;
    unsigned int maxStep() const;
protected:
    void applyOn(const DAction& ac);
    void computeDState();
    void initState();
    
private:
    MCar* prob;
    QTable feedbacks;
};


}

#endif

#ifndef SARSALAMB_HPP
#define SARSALAMB_HPP


///
///\file SarsaLamb.hpp
///\brief Algorithme avec l'historique de Sarsa
///


#include "Q.hpp"
#include "LearnStat.hpp"
#include "sml/QLearningLamb.hpp"

namespace sml {

class SarsaLamb : public sml::LearnStat
{

public:
  
///
///\brief Construction
///\param stmp : le modèle d'état
/// 	  atmp : le modèle d'action
///       s : l'état
///       a : l'action
///       conf : la configuration d'apprentissage
    SarsaLamb(const StateTemplate* stmp, const ActionTemplate* atmp, DState& s, DAction& a, const LearnConfig& conf);
    
    
    void observeTutor(DState& sp, DAction& ac, double r, float lrate, float discount, float lambda, bool accumulative);
    
    
    
///
///\brief Retourner l'action à faire selon l'algorithme avec l'historique de Sarsa
///\param s : le nouvel état
/// 	  r : la récompense
///       lrate : le taux d'apprentissage
///	  epsilon : politique "epsilon-greedy"
///	  discount : importance du prochain état de la récompense 
///	  lambda : importance de l'historique
///	  accumulative : si les traces est accumulative ou non
    DAction* learn(DState& s, double r, float lrate, float epsilon, float discount, float lambda, bool accumulative);
 
    
///
///\brief Retourner l'action à faire selon l'algorithme Sarsa sans apprentissage 
///\param s : le nouvel état
    DAction* decision(DState& s);
    
protected:
  
///
///\brief Sauvegarder ce que l'algorithme a appris
///\param xml : le fichier XML
    void save(boost::archive::xml_oarchive* xml);
    
    
///
///\brief Charger ce que l'algorithme a appris
///\param xml : le fichier XML
    void load(boost::archive::xml_iarchive* xml);
private:
    QTable Q, N;
    const ActionTemplate* atmp;
    DState* s;
    DAction* a;

    std::set< std::pair<DState*, DAction*>, HistoryComparator > history;
};

}
#endif // SARSALAMB

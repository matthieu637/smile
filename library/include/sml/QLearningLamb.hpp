#ifndef QLEARNINGLAMB_HPP
#define QLEARNINGLAMB_HPP


///
///\file QLearningLamb.hpp
///\brief Algorithme avec l'historique de QLearning
///


#include "Q.hpp"
#include "LearnStat.hpp"

namespace sml {

///
///\brief Classe pour trier l'arbre de l'historique
class HistoryComparator
{
public:
    bool operator()(const std::pair<DState*, DAction*>& p1, const std::pair<DState*, DAction*>& p2) const
    {
        if(*p1.first == *p2.first)
            return *p1.second < *p2.second;
        return *p1.first < *p2.first;
    }
};


class QLearningLamb : public sml::LearnStat
{

public:
  
///
///\brief Constructeur
///\param stmp : le modèle d'état
/// 	  atmp : le modèle d'action
///       s : l'état
///       a : l'action
///       conf : la configuration d'apprentissage
    QLearningLamb(const StateTemplate* stmp, const ActionTemplate* atmp, const DState& s, const DAction& a, const LearnConfig& conf={false, 0, 0});
    
    ~QLearningLamb();
    
///
///\brief Retourner l'action à faire selon l'algorithme avec l'historique de QLearning
///\param s : le nouvel état
/// 	  r : la récompense
///       lrate : le taux d'apprentissage
///	  epsilon : politique "epsilon-greedy"
///	  discount : importance du prochain état de la récompense 
///	  lambda : importance de l'historique
///	  accumulative : si les traces est accumulative ou non
    DAction* learn(DState& s, double r, float lrate, float epsilon, float discount, float lambda, bool accumulative);
 
    
///
///\brief Retourner l'action à faire selon l'algorithme QLearning sans apprentissage 
///\param s : le nouvel état
    DAction* decision(DState& s);
    
    
    void clear_history(const DState& ds, const DAction& da);
    
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
#endif // QLEARNINGLAMB

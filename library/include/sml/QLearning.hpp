#ifndef QLEARNING_HPP
#define QLEARNING_HPP

///
///\file QLearning.hpp
///\brief Algorithme générique de QLearning
///

#include "Q.hpp"
#include "Policy.hpp"

namespace sml {

class QLearning : public DPolicy
{

public:
  
///
///\brief Constructeur
///\param stmp : le modèle d'état
/// 	  atmp : le modèle d'action
///       s : l'état
///       a : l'action
///       conf : la configuration d'apprentissage
    QLearning(const StateTemplate* stmp, const ActionTemplate* atmp, const DState& s, const DAction& a, RLParam param, StrategyEffectsAdvice sea=None);
    
    QLearning(const QLearning& q);
    
    virtual ~QLearning();
    
///
///\brief Retourner l'action à faire selon l'algorithme de QLearning
///\param s : le nouvel état
/// 	  r : la récompense
///       lrate : le taux d'apprentissage
///	  epsilon : politique "epsilon-greedy"
///	  discount : importance du prochain état de la récompense 
    LearnReturn _learn(const DState& s, double r, bool);
    
///
///\brief Retourner l'action à faire selon l'algorithme QLearning sans apprentissage 
///\param s : le nouvel état
    DAction* decision(const DState& s, bool greedy);
    
    
    void startEpisode(const DState& ds, const DAction& da);
    
    void should_done(const DState& s, const DAction& a);
    void had_choosed(const DState&, const DAction&, double, bool, bool);
    bool learn_gotGreeding() const;
    void should_do(const DState& s, const DAction& a, double reward, bool);
    float getStateImportance(const DState& s);
    const QTable& getPolicy();
    Policy<DState>* copyPolicy();
    
    
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
    QTable Q, P;
    const ActionTemplate* atmp;
    const DState* ds;
    const DAction* da;

};

}
#endif // QLEARNING_HPP

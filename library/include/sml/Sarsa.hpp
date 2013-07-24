#ifndef SARSA_HPP
#define SARSA_HPP

///
///\file Sarsa.hpp
///\brief Algorithme générique de SARSA
///

#include "Q.hpp"
#include "LearnStat.hpp"
#include "Policy.hpp"

namespace sml {

class Sarsa : public LearnStat, public DPolicy
{
public:
///
///\brief Constructeur
///\param stmp : le modèle d'état
/// 	  atmp : le modèle d'action
///       s : l'état
///       a : l'action
///       conf : la configuration d'apprentissage
    Sarsa(const StateTemplate* stmp, const ActionTemplate* atmp, const DState& s, const DAction& a, const LearnConfig& conf={false,0,0});
    
    Sarsa(const Sarsa& q);
    
    virtual ~Sarsa();
    
///
///\brief Retourner l'action à faire selon l'algorithme de Sarsa
///\param s : le nouvel état
/// 	  r : la récompense
///       lrate : le taux d'apprentissage
///	  epsilon : politique "epsilon-greedy"
///	  discount : importance du prochain état de la récompense 
    DAction* learn(const DState& s, double r, float lrate, float epsilon, float discount);
    
///
///\brief Retourner l'action à faire selon l'algorithme Sarsa sans apprentissage 
///\param s : le nouvel état
    DAction* decision(const DState& s, float epsilon=0.L);
    
    
    void clear_history(const DState& ds, const DAction& da);
    
    void should_done(const DState& s, const DAction& a);
    void should_do(const DState& s, const DAction& a);
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
#endif // SARSA_HPP

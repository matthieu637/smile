#ifndef LEARNSTAT_H
#define LEARNSTAT_H

///
///\file LearnStat.hpp
///\brief Statistique sur l'apprentissage
///
///

#include <bib/XMLEngine.hpp>
#include <string>
using std::string;

namespace sml {
 
  
///\struct LearnConfig
///\brief Configuration d'un apprentissage
struct LearnConfig{
    bool restrictive_learning; //on empêche de sauvegarder l'algorithme si les performances sont trop mauvaises
    
    float ratio_limit;  //paramètres pour definir si les performances sont trop mauvaises
    float const_limit; 
};


class LearnStat
{
public:
  
  
///
///\brief Constructeur
///\param conf : la configuration 
    LearnStat(const LearnConfig& conf);
    
///
///\brief Sauvegarder les données de l'algorithme
///\param chemin : l'adresse où on sauvegarde     
    void write(const string& chemin);
    
    
///
///\brief Charger les données de l'algorithme
///\param chemin : l'adresse où on charge
    void read(const string& chemin);
    
///
///\brief Envoyer le performance
///\param globalReward : la somme de récompenses      
    void setPerf(double globalReward);
    
///
///\brief Augmenter le numéro de l'épisode 
    void increaseEpisode();
    
///
///\brief Verifier si on continu d'apprendre ou pas
///\param Perf : le performance
///\return False si le performance est mauvais, True sinon
    bool keepLearning(int perf);

protected:
  
///
///\brief Sauvegarder ce que l'algorithme a appris
///\param xml : le fichier XML
    virtual void save(boost::archive::xml_oarchive* xml) = 0;
  
///
///\brief Charger ce que l'algorithme a appris
///\param xml : le fichier XML
    virtual void load(boost::archive::xml_iarchive* xml) = 0;

    int performance;
    int last_performance;
    int episode;
    
    bool performance_set;
    bool read_before;
    
    LearnConfig conf;
};

}

#endif // LEARNSTAT_H

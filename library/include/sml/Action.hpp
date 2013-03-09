
#ifndef ACTION_HPP
#define ACTION_HPP

#include <string>
#include <list>
#include <unordered_map>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/map.hpp>


using std::string;
using boost::serialization::make_nvp;

namespace sml{

class ActionTemplate{
public:
  ActionTemplate(const std::initializer_list<string>& names);
  int indexFor(const string& name) const;
  int actionNumber() const;
  bool operator==(const ActionTemplate& ac) const;
private :
  //std::list<string> actionNames;
  std::unordered_map< string, int> actionNames;
};

typedef ActionTemplate StateTemplate;
  
class Action{
  
private:  
    double *values;
    ActionTemplate *templ;
};

class DAction{
  
public:
  //DAction();
  DAction(const ActionTemplate* temp, const std::initializer_list<int>& vals);
  ~DAction();
//   int get( int index) const;
  int get(const string& name) const;
  int operator[](const string& name) const; 
  int& operator[](const string& name);
  bool operator==(const DAction& ac) const;
  
  struct hashfunc {
    size_t operator() (const DAction& ac) const {
            return 1;
      }
  };
    
  /*
  template<class Archive>
  void serialize(Archive& ar, const unsigned int version)
  {
    (void) version;
    ar & make_nvp("values", values);
  }*/

private:
    int *values = nullptr;
    const ActionTemplate *templ;
};

typedef DAction DState;

}

#endif

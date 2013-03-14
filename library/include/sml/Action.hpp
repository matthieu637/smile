
#ifndef ACTION_HPP
#define ACTION_HPP

#include <string>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/split_member.hpp>
#include "sml/ActionTemplate.hpp"

using std::string;
using boost::serialization::make_nvp;

namespace sml {

class Action {

private:
    double *values;
    ActionTemplate *templ;
};

class DAction {

public:
    friend class boost::serialization::access;
    DAction();//empty constructor for serialization
    DAction(const ActionTemplate* temp, const std::list<int>& vals);
    DAction(const ActionTemplate* temp, int value);
    ~DAction();
    int get( int index) const;
    int get(const string& name) const;
    int operator[](const string& name) const;
    void set(const string& name, int value);
    bool operator==(const DAction& ac) const;
    bool operator<(const DAction& ac) const;
    unsigned int hash() const;
    void print(std::ostream &flux) const;


    BOOST_SERIALIZATION_SPLIT_MEMBER()
    template<class Archive>
    void save(Archive& ar, const unsigned int version) const {
        (void) version;
        ar << make_nvp("Template", this->templ);

        for(int i = 0 ; i< templ->actionNumber(); i++)
            ar << make_nvp("values", values[i]);
    }

    template<class Archive>
    void load(Archive& ar, const unsigned int version) {
        (void) version;
        ar >> make_nvp("Template", this->templ);

        values = new int[this->templ->actionNumber()];
        for(int i = 0 ; i< templ->actionNumber(); i++)
            ar >> make_nvp("values", values[i]);
    }

private:
    void computehash();

    int *values = nullptr;
    const ActionTemplate *templ;
    int hashmem;
};

std::ostream& operator<< (std::ostream& stream, const sml::DAction& ac);

typedef DAction DState;

}

#endif

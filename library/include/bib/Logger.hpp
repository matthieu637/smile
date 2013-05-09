#ifndef LOGGER_HPP
#define LOGGER_HPP

///
///\file Logger.hpp
///\brief Singleton de Logger afin d'écrire des logs
///\example Dans LoggerTest.hpp
///
///

#include <iostream>
#include <sstream>
#include <string>
#include "Singleton.hpp"


#define LOG_DEBUG(stream) \
  bib::Logger::getInstance()->isEnabled(bib::Logger::DEBUGGING) && std::cout << "DEBUG :" << __FILE__ << "."<< __LINE__ << " : " << stream << std::endl

#define LOG_DEBUGS(stream) \
  bib::Logger::getInstance()->isEnabled(bib::Logger::DEBUGGING) && std::cout << "DEBUG :" << __FILE__ << "."<< __LINE__ << " : " << stream
  
#define LOG_DEBUGC(stream) \
  bib::Logger::getInstance()->isEnabled(bib::Logger::DEBUGGING) && std::cout << stream 

#define LOG_DEBUGE(stream) \
  bib::Logger::getInstance()->isEnabled(bib::Logger::DEBUGGING) && std::cout << std::endl
  
#define LOG_INFO(stream) \
  bib::Logger::getInstance()->isEnabled(bib::Logger::INFO) && std::cout << "INFO :" << __FILE__ << "."<< __LINE__ << " : " << stream << std::endl

#define LOG_WARNING(stream) \
  bib::Logger::getInstance()->isEnabled(bib::Logger::WARNING) && std::cout << "WARNING :" << __FILE__ << "."<< __LINE__ << " : " << stream << std::endl

#define LOG_ERROR(stream) \
  bib::Logger::getInstance()->isEnabled(bib::Logger::ERROR) && std::cout << "ERROR :" << __FILE__ << "."<< __LINE__ << " : " << stream << std::endl



namespace bib {

class Logger : public Singleton<Logger> {
    friend class Singleton<Logger>;

public:
    enum LogLevel { DEBUGGING, INFO, WARNING, ERROR };

    bool isEnabled(LogLevel l) {
        return level <= l;
    }

    void setLevel(LogLevel l) {
        level = l;
    }
    
    template <class T>
    static inline void PRINT_ELEMENTS (const T& coll, const char* optcstr="")
    {
        typename T::const_iterator pos;

        LOG_DEBUGS(optcstr);
        for (pos=coll.begin(); pos!=coll.end(); ++pos)
            LOG_DEBUGC(*pos << ", ");
        
        LOG_DEBUGE();
    }
    
    template <class T>
    static inline void PRINT_ELEMENTS (const T& coll, int length, const char* optcstr="")
    {
        LOG_DEBUGS(optcstr);
        for (int i=0;i<length;i++)
            LOG_DEBUGC(coll[i] << ", ");
        
        LOG_DEBUGE();
    }

protected:
    Logger():level(DEBUGGING) {}

private :
    LogLevel level;
};

}


#endif

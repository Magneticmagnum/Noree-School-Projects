#ifndef LOGGERSETUP_H_
#define LOGGERSETUP_H_

#include <cxxtest/GlobalFixture.h>
#include <iostream>
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>

using namespace std;
using log4cxx::Logger;
using log4cxx::LoggerPtr;
using log4cxx::PropertyConfigurator;
using log4cxx::File;

class LoggerSetup : public CxxTest::GlobalFixture
{
public:
    bool setUpWorld() 
    {
	// set up the logger
	LoggerPtr log(Logger::getLogger("Setup"));
	PropertyConfigurator::configure(File("log.config"));
	LOG4CXX_INFO(log, "Starting test");
	return true;
    }

    bool tearDownWorld() 
    {
	LoggerPtr log(Logger::getLogger("Setup"));
	LOG4CXX_INFO(log, "Leaving.");
	return true;
    }
};

//
// We can rely on this file being included exactly once
// and declare this global variable in the header file.
//
static LoggerSetup loggerSetup;
#endif /*LOGGERSETUP_H_*/

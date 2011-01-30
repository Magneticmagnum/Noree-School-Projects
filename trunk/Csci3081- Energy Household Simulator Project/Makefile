# $Id: Exp $
# makefile for 3081 group project Phase 1 Fall 2010
# module unload soft/gcc/4.2

OS = $(shell uname)
CPP = g++
CFLAGS = -fprofile-arcs -ftest-coverage -Wno-write-strings -g -I ../include #suppreses "depreceated conversion from string to constant"
LFLAGS = $(CFLAGS) -L ../lib -llog4cxx -lapr-1 -laprutil-1 -lboost_regex
CXX_DIR = ./libs/cxxtest
CXX = $(CXX_DIR)/cxxtestgen.pl
ifeq (CYGWIN, $(findstring CYGWIN, $(OS)))
	LFLAGS = $(CFLAGS) -L/usr/local/lib -lapr-1.dll -laprutil-1.dll -L ../lib -llog4cxx -lapr-1 -laprutil-1 -lboost_regex
endif

.SUFFIXES : .o .cpp .c
.cpp.o :
	$(CPP) -c -g $(CFLAGS) -ggdb3 $<

TESTS = tests/utilitiesTest.h  tests/PersonTest.h tests/SchedulerTest.h tests/RefrigeratorTest.h tests/ToasterTest.h tests/WaterHeaterTest.h tests/DishwasherTest.h tests/MicrowaveTest.h tests/OvenTest.h tests/TelevisionTest.h tests/StoveTest.h tests/ComputerTest.h tests/LightsTests.h tests/ModelTest.h tests/ShowerTest.h tests/HouseTest.h
SOURCES = src/Refrigerator.cpp src/Scheduler.cpp src/Model.cpp src/Toaster.cpp src/WaterHeater.cpp src/Dishwasher.cpp src/Microwave.cpp src/Oven.cpp src/Television.cpp src/Person.cpp src/Stove.cpp src/utilities.cpp src/Computer.cpp src/Lights.cpp src/Shower.cpp src/House.cpp
OBJS = Refrigerator.o Scheduler.o Model.o Toaster.o WaterHeater.o Dishwasher.o Microwave.o Oven.o Television.o Person.o Stove.o utilities.o Computer.o Lights.o Shower.o House.o
DOCS = libs/MainPage.h Doxyfile
VPATH = src:lib:test

# Build the executable
simulator: $(OBJS) 
	$(CPP) $(LFLAGS) $(OBJS) src/simulator.cpp -o simulator

all: simulator run-tests doc test-coverage

# Run the unit tests
run-tests: runner
	rm --force *.gcda
	LD_LIBRARY_PATH=../libs; export LD_LIBRARY_PATH; ./runner
	rm --force *.test
	
#Test coverage statistics
test-coverage: run-tests
	gcov -b $(SOURCES) | grep "^File '[^/]" --after-context 1 --color=NEVER >coverage.txt

# Generate the test runner
runner.cpp: $(TESTS) src/simulator.cpp
	$(CXX) --error-printer -o runner.cpp $(TESTS)

#Build the test runner
runner: $(TESTS) $(OBJS) libs/LoggerSetup.h
	$(CXX) --include=libs/LoggerSetup.h --error-printer -o runner.cpp $(TESTS)
	$(CPP) -I$(CXX_DIR) $(LFLAGS) -o runner runner.cpp $(OBJS)

# Generate documentation - will need to add the html folder pathwat to the Doxyfile
doc: libs/Doxyfile $(SOURCES) src/simulator.cpp
	doxygen libs/Doxyfile

# Clean Up
clean:
	rm -rf latex doc *.o logfile.txt log1.txt testlog.txt simulator runner* *.gcov *.gcno coverage.txt *.gcda *.log *.o
	
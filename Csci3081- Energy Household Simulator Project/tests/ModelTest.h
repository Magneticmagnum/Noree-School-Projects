#ifndef MODELTEST_H_
#define MODELTEST_H_

#include <cxxtest/TestSuite.h>

#include "../libs/Model.h"

class ModelTest: public CxxTest::TestSuite {
public:

	void test_model_Model() {
		Model m;
		m.activate();
		m.tick();
		m.activate(1.2, 1.2);
		m.activate(1, 1);
		m.activate(1.2);
	}


};

#endif /* MODELTEST_H_ */

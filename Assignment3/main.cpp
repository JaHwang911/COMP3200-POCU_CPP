#include <iostream>
#include <limits>
#include <cassert>
#include <stack>

#include "Test.h"

using namespace assignment3;

int main()
{
	TestOfficial();
	TestWiki1();
	TestWiki2();
	TestSmartStack();
	TestSmartQueue();
	TestQueueStack();

	std::cout << "No prob" << std::endl;
	return 0;
}
#include "pch.h"
#include "CppUnitTest.h"
#include "behavior_tree.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BehaviorTreeTest
{
	TEST_CLASS(BehaviorTreeTest)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			std::shared_ptr<BehaviorTree> pTree(new BehaviorTree());
		}
	};
}

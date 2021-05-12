#include "pch.h"
#include "CppUnitTest.h"
#include "behavior_tree.h"

#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BehaviorTreeTest
{
	TEST_CLASS(BehaviorTreeTest)
	{
	private:
		// coutの取得用
		std::stringbuf  buf;							// stringstream用のstreambuf
		std::streambuf* prev = nullptr;
		std::string str;								// cout で出力されたものを保持する

		// 前処理
		void pre_process()
		{
			prev = std::cout.rdbuf(&buf);				// streambufの置き換え
		}

		// 後処理
		void post_process()
		{
			std::cout.rdbuf(prev);                      // streambufを戻す
			str = buf.str();
		}

		const char* get_cout() const { return str.c_str(); }

	public:
		
		// +------------+
		// | Composite  |
		// +------------+
		//       |
		// +------------+
		// |テストです。|
		// +------------+
		//
		TEST_METHOD(TestMethodText)
		{
			pre_process();

			// Behevior Tree の設定
			std::shared_ptr<BehaviorTree> pTree(new BehaviorTree());
			std::shared_ptr<MessageAction> pShowTest(new MessageAction());
			pShowTest->set_message("テストです。");
			pTree->add_child(pShowTest.get());

			pTree->update();

			post_process();

			// 検証
			Assert::AreEqual("テストです。\n", get_cout(), "メッセージが違います");
		}

		//               +------------+
		//               | Composite  |
		//               +------------+
		//       ／　　　　　　 |　　　　　＼
		// +------------+ +----------+ +------------+
		// |  着替える  | |朝食をとる| |  歯を磨く  |
		// +------------+ +----------+ +------------+
		//
		TEST_METHOD(TestMethodText3)
		{
			pre_process();

			// Behevior Tree の設定
			std::shared_ptr<BehaviorTree> pTree(new BehaviorTree());
			std::shared_ptr<MessageAction> pShowTest1(new MessageAction());
			pShowTest1->set_message("着替える");
			pTree->add_child(pShowTest1.get());
			std::shared_ptr<MessageAction> pShowTest2(new MessageAction());
			pShowTest2->set_message("朝食をとる");
			pTree->add_child(pShowTest2.get());
			std::shared_ptr<MessageAction> pShowTest3(new MessageAction());
			pShowTest3->set_message("歯を磨く");
			pTree->add_child(pShowTest3.get());

			pTree->update();

			post_process();

			// 検証
			Assert::AreEqual("着替える\n朝食をとる\n歯を磨く\n", get_cout(), "メッセージが違います");
		}

	};
}

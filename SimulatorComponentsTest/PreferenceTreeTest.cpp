#include "stdafx.h"
#include "CppUnitTest.h"
#include "PreferenceTree.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Test
{
	TEST_CLASS(PreferenceTreeTest)
	{
	public:

		TEST_METHOD(PreferenceTree_get_support_of)
		{
			std::shared_ptr<User::PreferenceTree> preference = std::make_shared<User::PreferenceTree>();
			preference->add_sequence_counter({ "A", "B" });
			preference->add_sequence_counter({ "A", "C" });
			preference->add_sequence_counter({ "B", "C", "D" });

			double actual = preference->get_support_of({ "A" });
			Assert::AreEqual(2.0 / 3.0, actual);
			
			actual = preference->get_support_of({ "B", "C", "D" });
			Assert::AreEqual(1.0 / 3.0, actual);
		}

		TEST_METHOD(PreferenceTree_max_depth)
		{
			std::shared_ptr<User::PreferenceTree> preference = std::make_shared<User::PreferenceTree>();
			preference->add_sequence_counter({ "A", "B" });
			preference->add_sequence_counter({ "A", "C" });
			preference->add_sequence_counter({ "B", "C", "D" });
			preference->add_sequence_counter({ "A", "B", "D", "E", "F", "G" });
			int actual = preference->max_depth();
			Assert::AreEqual(6, actual);
		}

		TEST_METHOD(PreferenceTree_similarity)
		{
			std::shared_ptr<User::PreferenceTree> preference = std::make_shared<User::PreferenceTree>();
			preference->add_sequence_counter({ "A", "B" });
			preference->add_sequence_counter({ "A", "B" });
			preference->add_sequence_counter({ "A", "C" });
			preference->add_sequence_counter({ "B", "C", "D" });

			std::shared_ptr<User::PreferenceTree> preference2 = std::make_shared<User::PreferenceTree>();;
			preference2->add_sequence_counter({ "A", "B" });
			preference2->add_sequence_counter({ "A", "C" });

			double similarity = User::similarity(*preference, *preference2);
			Assert::AreEqual(1.0 - (0.25 * 5 / 6), similarity);
		}
	};
}

#include "stdafx.h"
#include "CppUnitTest.h"
#include "Identifiable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilsTest
{
	TEST_CLASS(IdentifiableTest)
	{
	public:

		TEST_METHOD(Identifiable_Constructor)
		{
			Identifiable id1(2UL);
			Assert::AreEqual(2L, id1.get_id());
		}

		TEST_METHOD(Identifiable_operators1) 
		{
			Identifiable id1(1UL);
			Identifiable id2(2UL);
			Identifiable id3(2UL);
			std::shared_ptr<Identifiable> id4 = std::make_shared<Identifiable>(2UL);
			std::shared_ptr<Identifiable> id5 = std::make_shared<Identifiable>(2UL);
			std::shared_ptr<Identifiable> id6 = std::make_shared<Identifiable>(3UL);

			Assert::IsFalse(id1 == id2);
			Assert::IsTrue(id1 != id2);
			Assert::IsTrue(id2 == id3);
			Assert::IsFalse(id2 != id3);
			
			Assert::IsTrue(id4 == 2UL);
			Assert::IsFalse(id4 != 2UL);
			Assert::IsTrue(id1 == 1UL);
			Assert::IsFalse(id1 != 1UL);
		}

		TEST_METHOD(Identifiable_operators2) 
		{
			Identifiable id1(1UL);
			Identifiable id2(2UL);
			Identifiable id3(2UL);
			
			Assert::IsFalse(id1 > id2);
			Assert::IsTrue(id1 < id2);
			Assert::IsTrue(id2 <= id3);
			Assert::IsFalse(id2 < id3);
			Assert::IsTrue(id2 >= id3);
			Assert::IsFalse(id2 > id3);

			Assert::IsFalse(id1 > 2UL);
			Assert::IsTrue(id1 < 2UL);
			Assert::IsTrue(id2 <= 2UL);
			Assert::IsFalse(id2 < 1UL);
			Assert::IsTrue(id2 >= 2UL);
			Assert::IsFalse(id2 > 3UL);
		}

		TEST_METHOD(Identifiable_operators3) 
		{
			std::shared_ptr<Identifiable> id1 = std::make_shared<Identifiable>(1UL);
			std::shared_ptr<Identifiable> id2 = std::make_shared<Identifiable>(2UL);
			std::shared_ptr<Identifiable> id3 = std::make_shared<Identifiable>(2UL);
			Assert::IsFalse(id1 > 2UL);
			Assert::IsTrue(id1 < 2UL);
			Assert::IsTrue(id2 <= 2UL);
			Assert::IsFalse(id2 < 1UL);
			Assert::IsTrue(id2 >= 2UL);
			Assert::IsFalse(id2 > 3UL);
		}
	};
}

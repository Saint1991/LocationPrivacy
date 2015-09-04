#pragma once
#include "stdafx.h"
#include "CppUnitTest.h"
#include "Identifiable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilsTest
{
	TEST_CLASS(IdentifiableTest)
	{
	public:

		TEST_METHOD(Identifiable_Constructor1)
		{
			Identifiable<long> id1(2UL);
			Assert::AreEqual(2L, id1.get_id());
		}

		TEST_METHOD(Identifiable_Constructor2)
		{
			Identifiable<std::string> id1("a");
			std::string expected = "a";
			Assert::AreEqual(expected, id1.get_id());
		}

		TEST_METHOD(Identifiable_operators1) 
		{
			Identifiable<long> id1(1UL);
			Identifiable<long> id2(2UL);
			Identifiable<long> id3(2UL);
			std::shared_ptr<Identifiable<long>> id4 = std::make_shared<Identifiable<long>>(2UL);
			std::shared_ptr<Identifiable<long>> id5 = std::make_shared<Identifiable<long>>(2UL);
			std::shared_ptr<Identifiable<long>> id6 = std::make_shared<Identifiable<long>>(3UL);

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
			Identifiable<long> id1(1UL);
			Identifiable<long> id2(2UL);
			Identifiable<long> id3(2UL);
			
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
			std::shared_ptr<Identifiable<long>> id1 = std::make_shared<Identifiable<long>>(1UL);
			std::shared_ptr<Identifiable<long>> id2 = std::make_shared<Identifiable<long>>(2UL);
			std::shared_ptr<Identifiable<long>> id3 = std::make_shared<Identifiable<long>>(2UL);
			Assert::IsFalse(id1 > 2UL);
			Assert::IsTrue(id1 < 2UL);
			Assert::IsTrue(id2 <= 2UL);
			Assert::IsFalse(id2 < 1UL);
			Assert::IsTrue(id2 >= 2UL);
			Assert::IsFalse(id2 > 3UL);
		}

		TEST_METHOD(Identifiable_operators4)
		{
			Identifiable<std::string> id1("a");
			Identifiable<std::string> id2("b");
			Identifiable<std::string> id3("aa");
			Identifiable<std::string> id4("a");

			Assert::IsTrue(id1 < id2 && id1 < id3);
			Assert::IsTrue(id2 >= id3);
			Assert::IsTrue(id1 == id4);
		}
	};
}

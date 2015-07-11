#include "stdafx.h"
#include "CppUnitTest.h"
#include "Identifiable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilsTest
{
	TEST_CLASS(IdentifiableTest)
	{
	public:
		
		TEST_METHOD(get_id)
		{
			int expected = 8;
			Identifiable id(8);
			int actual = id.get_id();
			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(operator_bl_eq1)
		{
			Identifiable id1(8);
			Identifiable id2(8);
			Assert::IsTrue(id1 == id2);
		}

		TEST_METHOD(operator_bl_eq2)
		{
			Identifiable id1(8);
			Identifiable id2(7);
			Assert::IsFalse(id1 == id2);
		}

		TEST_METHOD(operator_bl_eq3)
		{
			Identifiable id1(8);
			unsigned long id2 = 8;
			Assert::IsTrue(id1 == id2);
		}

		TEST_METHOD(operator_bl_eq4)
		{
			Identifiable id1(8);
			unsigned long id2 = 0;
			Assert::IsFalse(id1 == id2);
		}

		TEST_METHOD(operator_bl_neq1)
		{
			Identifiable id1(8);
			Identifiable id2(8);
			Assert::IsFalse(id1 != id2);
		}

		TEST_METHOD(operator_bl_neq2)
		{
			Identifiable id1(8);
			Identifiable id2(7);
			Assert::IsTrue(id1 != id2);
		}

		TEST_METHOD(operator_bl_neq3)
		{
			Identifiable id1(8);
			unsigned id2 = 8;
			Assert::IsFalse(id1 != id2);
		}

		TEST_METHOD(operator_bl_neq4)
		{
			Identifiable id1(8);
			unsigned long id2 = 1;
			Assert::IsTrue(id1 != id2);
		}

		TEST_METHOD(operator_less1)
		{
			Identifiable id1(7);
			Identifiable id2(8);
			Assert::IsTrue(id1 < id2);
		}

		TEST_METHOD(operator_less2)
		{
			Identifiable id1(7);
			Identifiable id2(7);
			Assert::IsFalse(id1 < id2);
		}

		TEST_METHOD(operator_less3)
		{
			Identifiable id1(7);
			Identifiable id2(6);
			Assert::IsFalse(id1 < id2);
		}

		TEST_METHOD(operator_less4)
		{
			Identifiable id1(7);
			unsigned long id2(8);
			Assert::IsTrue(id1 < id2);
		}

		TEST_METHOD(operator_less5)
		{
			Identifiable id1(7);
			unsigned long id2 = 7;
			Assert::IsFalse(id1 < id2);
		}

		TEST_METHOD(operator_less6)
		{
			Identifiable id1(7);
			unsigned long id2 = 6;
			Assert::IsFalse(id1 < id2);
		}

		TEST_METHOD(operator_greater1)
		{
			Identifiable id1(8);
			unsigned long id2 = 7;
			Assert::IsTrue(id1 > id2);
		}

		TEST_METHOD(operator_greater2)
		{
			Identifiable id1(7);
			unsigned long id2 = 7;
			Assert::IsFalse(id1 > id2);
		}

		TEST_METHOD(operator_greater3)
		{
			Identifiable id1(6);
			unsigned long id2 = 7;
			Assert::IsFalse(id1 > id2);
		}

		TEST_METHOD(operator_greater4)
		{
			Identifiable id1(8);
			Identifiable id2(7);
			Assert::IsTrue(id1 > id2);
		}

		TEST_METHOD(operator_greater5)
		{
			Identifiable id1(7);
			Identifiable id2(7);
			Assert::IsFalse(id1 > id2);
		}

		TEST_METHOD(operator_greater6)
		{
			Identifiable id1(6);
			Identifiable id2(7);
			Assert::IsFalse(id1 > id2);
		}

		TEST_METHOD(operator_leq1)
		{
			Identifiable id1(6);
			Identifiable id2(7);
			Assert::IsTrue(id1 <= id2);
		}

		TEST_METHOD(operator_leq2)
		{
			Identifiable id1(7);
			Identifiable id2(7);
			Assert::IsTrue(id1 <= id2);
		}

		TEST_METHOD(operator_leq3)
		{
			Identifiable id1(8);
			Identifiable id2(7);
			Assert::IsFalse(id1 <= id2);
		}

		TEST_METHOD(operator_leq4)
		{
			Identifiable id1(6);
			unsigned long id2 = 7;
			Assert::IsTrue(id1 <= id2);
		}

		TEST_METHOD(operator_leq5)
		{
			Identifiable id1(7);
			unsigned long id2 = 7;
			Assert::IsTrue(id1 <= id2);
		}

		TEST_METHOD(operator_leq6)
		{
			Identifiable id1(8);
			unsigned long id2 = 7;
			Assert::IsFalse(id1 <= id2);
		}

		TEST_METHOD(operator_geq1)
		{
			Identifiable id1(8);
			unsigned long id2 = 7;
			Assert::IsTrue(id1 >= id2);
		}

		TEST_METHOD(operator_geq2)
		{
			Identifiable id1(7);
			unsigned long id2 = 7;
			Assert::IsTrue(id1 >= id2);
		}

		TEST_METHOD(operator_geq3)
		{
			Identifiable id1(6);
			unsigned long id2 = 7;
			Assert::IsFalse(id1 >= id2);
		}

		TEST_METHOD(operator_geq4)
		{
			Identifiable id1(8);
			Identifiable id2(7);
			Assert::IsTrue(id1 >= id2);
		}

		TEST_METHOD(operator_geq5)
		{
			Identifiable id1(7);
			Identifiable id2(7);
			Assert::IsTrue(id1 >= id2);
		}

		TEST_METHOD(operator_geq6)
		{
			Identifiable id1(6);
			Identifiable id2(7);
			Assert::IsFalse(id1 >= id2);
		}

		TEST_METHOD(operator_bl_eq_ptr1)
		{
			std::shared_ptr<Identifiable> id1 = std::make_shared<Identifiable>(7);
			unsigned long id2 = 7;
			Assert::IsTrue(id1 == id2);
		}

		TEST_METHOD(operator_bl_eq_ptr2)
		{
			std::shared_ptr<Identifiable> id1 = std::make_shared<Identifiable>(7);
			unsigned long id2 = 8;
			Assert::IsFalse(id1 == id2);
		}

		TEST_METHOD(operator_bl_neq_ptr1)
		{
			std::shared_ptr<Identifiable> id1 = std::make_shared<Identifiable>(7);
			unsigned long id2 = 8;
			Assert::IsTrue(id1 != id2);
		}

		TEST_METHOD(operator_bl_neq_ptr2)
		{
			std::shared_ptr<Identifiable> id1 = std::make_shared<Identifiable>(8);
			unsigned long id2 = 8;
			Assert::IsFalse(id1 != id2);
		}

		TEST_METHOD(operator_less_ptr1) 
		{
			std::shared_ptr<Identifiable> id1 = std::make_shared<Identifiable>(7);
			unsigned long id2 = 8;
			Assert::IsTrue(id1 < id2);
		}

		TEST_METHOD(operator_less_ptr2)
		{
			std::shared_ptr<Identifiable> id1 = std::make_shared<Identifiable>(8);
			unsigned long id2 = 8;
			Assert::IsFalse(id1 < id2);
		}

		TEST_METHOD(operator_less_ptr3)
		{
			std::shared_ptr<Identifiable> id1 = std::make_shared<Identifiable>(9);
			unsigned long id2 = 8;
			Assert::IsFalse(id1 < id2);
		}

		TEST_METHOD(operator_grater_ptr1)
		{
			std::shared_ptr<Identifiable> id1 = std::make_shared<Identifiable>(9);
			unsigned long id2 = 8;
			Assert::IsTrue(id1 > id2);
		}

		TEST_METHOD(operator_grater_ptr2)
		{
			std::shared_ptr<Identifiable> id1 = std::make_shared<Identifiable>(8);
			unsigned long id2 = 8;
			Assert::IsFalse(id1 > id2);
		}

		TEST_METHOD(operator_grater_ptr3)
		{
			std::shared_ptr<Identifiable> id1 = std::make_shared<Identifiable>(7);
			unsigned long id2 = 8;
			Assert::IsFalse(id1 > id2);
		}

		TEST_METHOD(operator_leq_ptr1)
		{
			std::shared_ptr<Identifiable> id1 = std::make_shared<Identifiable>(7);
			unsigned long id2 = 8;
			Assert::IsTrue(id1 <= id2);
		}

		TEST_METHOD(operator_leq_ptr2)
		{
			std::shared_ptr<Identifiable> id1 = std::make_shared<Identifiable>(8);
			unsigned long id2 = 8;
			Assert::IsTrue(id1 <= id2);
		}

		TEST_METHOD(operator_leq_ptr3)
		{
			std::shared_ptr<Identifiable> id1 = std::make_shared<Identifiable>(9);
			unsigned long id2 = 8;
			Assert::IsFalse(id1 <= id2);
		}

		TEST_METHOD(operator_geq_ptr1)
		{
			std::shared_ptr<Identifiable> id1 = std::make_shared<Identifiable>(9);
			unsigned long id2 = 8;
			Assert::IsTrue(id1 >= id2);
		}

		TEST_METHOD(operator_geq_ptr2)
		{
			std::shared_ptr<Identifiable> id1 = std::make_shared<Identifiable>(8);
			unsigned long id2 = 8;
			Assert::IsTrue(id1 >= id2);
		}

		TEST_METHOD(operator_geq_ptr3)
		{
			std::shared_ptr<Identifiable> id1 = std::make_shared<Identifiable>(7);
			unsigned long id2 = 8;
			Assert::IsFalse(id1 >= id2);
		}





	};
}
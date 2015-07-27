#pragma once
#include "stdafx.h"
#include "CppUnitTest.h"
#include "IdentifiableCollection.h"
#include "Identifiable.h"
#include "Node.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilsTest
{
	TEST_CLASS(IdentifiableCollectionTest)
	{
	public:
		
		TEST_METHOD(assertion_check)
		{
			Collection::IdentifiableCollection<Identifiable> collection;
			Collection::IdentifiableCollection<Graph::Node<double, double>> collection2;
		}

		TEST_METHOD(add)
		{
			Collection::IdentifiableCollection<Identifiable> collection;
			Assert::IsTrue(collection.add(Identifiable(1L)));
			try {
				Assert::IsFalse(collection.add(Identifiable(1L)));
				Assert::Fail();
			} 
			catch (DuplicatedIdException e) {
				std::string message = e.what();
				Logger::WriteMessage(e.what());
				Assert::IsTrue(message == "Duplicated ID - 1");
			}
		}

		TEST_METHOD(get1) 
		{
			Collection::IdentifiableCollection<Identifiable> collection;

			std::shared_ptr<Identifiable> id1 = std::make_shared<Identifiable>(2);
			collection.add(id1);
			std::shared_ptr<Identifiable> actual = collection.get_by_id(2);
			Assert::IsTrue(id1 == actual);
		}

		TEST_METHOD(get2)
		{
			Collection::IdentifiableCollection<Identifiable> collection;

			std::shared_ptr<Identifiable> id2 = std::make_shared<Identifiable>(2);
			std::shared_ptr<Identifiable> id1 = std::make_shared<Identifiable>(1);
			
			collection.add(id2);
			std::shared_ptr<Identifiable> actual = collection.get_by_id(1);
			Assert::IsTrue(actual == nullptr);
		}

		TEST_METHOD(read1)
		{
			Collection::IdentifiableCollection<Identifiable> collection;

			std::shared_ptr<Identifiable> id2 = std::make_shared<Identifiable>(2);
			std::shared_ptr<Identifiable> id4 = std::make_shared<Identifiable>(4);
			std::shared_ptr<Identifiable> id1 = std::make_shared<Identifiable>(1);

			collection.add(id2);
			collection.add(id4);
			collection.add(id1);
		
			std::shared_ptr<Identifiable const> actual = collection.read_by_id(1);
			Assert::IsTrue(actual == 1);
		}

		TEST_METHOD(read4)
		{
			Collection::IdentifiableCollection<Identifiable> collection;

			std::shared_ptr<Identifiable> id2 = std::make_shared<Identifiable>(2);
			std::shared_ptr<Identifiable> id4 = std::make_shared<Identifiable>(4);
			std::shared_ptr<Identifiable> id1 = std::make_shared<Identifiable>(1);

			collection.add(id2);
			collection.add(id4);
			collection.add(id1);

			std::shared_ptr<Identifiable const> actual = collection.read_by_id(5);
			Assert::IsTrue(actual == nullptr);
		}

		TEST_METHOD(contains1)
		{
			Collection::IdentifiableCollection<Identifiable> collection;

			std::shared_ptr<Identifiable> id2 = std::make_shared<Identifiable>(2);
			std::shared_ptr<Identifiable> id4 = std::make_shared<Identifiable>(4);
			std::shared_ptr<Identifiable> id1 = std::make_shared<Identifiable>(1);

			collection.add(id2);
			collection.add(id4);
			collection.add(id1);

			bool actual = collection.contains(1);
			Assert::IsTrue(actual);
		}

		TEST_METHOD(contains2)
		{
			Collection::IdentifiableCollection<Identifiable> collection;

			std::shared_ptr<Identifiable> id2 = std::make_shared<Identifiable>(2);
			std::shared_ptr<Identifiable> id4 = std::make_shared<Identifiable>(4);
			std::shared_ptr<Identifiable> id1 = std::make_shared<Identifiable>(1);

			collection.add(id2);
			collection.add(id4);
			collection.add(id1);

			bool actual = collection.contains(3);
			Assert::IsFalse(actual);
		}

		TEST_METHOD(contains3)
		{
			Collection::IdentifiableCollection<Identifiable> collection;

			std::shared_ptr<Identifiable> id2 = std::make_shared<Identifiable>(2);
			std::shared_ptr<Identifiable> id4 = std::make_shared<Identifiable>(4);
			std::shared_ptr<Identifiable> id1 = std::make_shared<Identifiable>(1);

			collection.add(id2);
			collection.add(id4);
			collection.add(id1);

			bool actual = collection.contains(Identifiable(1));
			Assert::IsTrue(actual);
		}

		TEST_METHOD(contains4)
		{
			Collection::IdentifiableCollection<Identifiable> collection;

			std::shared_ptr<Identifiable> id2 = std::make_shared<Identifiable>(2);
			std::shared_ptr<Identifiable> id4 = std::make_shared<Identifiable>(4);
			std::shared_ptr<Identifiable> id1 = std::make_shared<Identifiable>(1);

			collection.add(id2);
			collection.add(id4);
			collection.add(id1);

			bool actual = collection.contains(Identifiable(3));
			Assert::IsFalse(actual);
		}

		TEST_METHOD(remove_by_id)
		{
			Collection::IdentifiableCollection<Identifiable> collection;

			std::shared_ptr<Identifiable> id2 = std::make_shared<Identifiable>(2);
			std::shared_ptr<Identifiable> id4 = std::make_shared<Identifiable>(4);
			std::shared_ptr<Identifiable> id1 = std::make_shared<Identifiable>(1);

			collection.add(id2);
			collection.add(id4);
			collection.add(id1);

			collection.remove_by_id(2);

			Assert::IsTrue(collection.contains(1));
			Assert::IsFalse(collection.contains(2));
			Assert::IsFalse(collection.contains(3));
			Assert::IsTrue(collection.contains(4));
		}

		TEST_METHOD(size)
		{
			Collection::IdentifiableCollection<Identifiable> collection;

			std::shared_ptr<Identifiable> id2 = std::make_shared<Identifiable>(2);
			std::shared_ptr<Identifiable> id4 = std::make_shared<Identifiable>(4);

			collection.add(id2);
			Assert::AreEqual((std::size_t)1, collection.size());
			
			collection.add(id4);
			Assert::AreEqual((std::size_t)2, collection.size());
			
			collection.remove_by_id(2);
			Assert::AreEqual((std::size_t)1, collection.size());
		}

		TEST_METHOD(clear)
		{
			Collection::IdentifiableCollection<Identifiable> collection;

			std::shared_ptr<Identifiable> id2 = std::make_shared<Identifiable>(2);
			std::shared_ptr<Identifiable> id4 = std::make_shared<Identifiable>(4);

			collection.add(id2);
			Assert::AreEqual((std::size_t)1, collection.size());

			collection.add(id4);
			Assert::AreEqual((std::size_t)2, collection.size());

			collection.clear();
			Assert::AreEqual((std::size_t)0, collection.size());
			Assert::IsFalse(collection.contains(2));
			Assert::IsFalse(collection.contains(4));
		}
		
	};
}
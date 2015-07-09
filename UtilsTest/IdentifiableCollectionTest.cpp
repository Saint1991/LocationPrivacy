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

		TEST_METHOD(find1) 
		{
			Collection::IdentifiableCollection<Identifiable> collection;

			std::shared_ptr<Identifiable> id1 = std::make_shared<Identifiable>(2);
			collection.add(id1);
			std::shared_ptr<Identifiable> actual = collection.find_by_id(2);
			Assert::IsTrue(id1 == actual);
		}

		TEST_METHOD(find2)
		{
			Collection::IdentifiableCollection<Identifiable> collection;

			std::shared_ptr<Identifiable> id2 = std::make_shared<Identifiable>(2);
			std::shared_ptr<Identifiable> id1 = std::make_shared<Identifiable>(1);
			
			collection.add(id2);
			std::shared_ptr<Identifiable> actual = collection.find_by_id(1);
			Assert::IsTrue(actual == nullptr);
		}

		TEST_METHOD(find3)
		{
			Collection::IdentifiableCollection<Identifiable> collection;

			std::shared_ptr<Identifiable> id2 = std::make_shared<Identifiable>(2);
			std::shared_ptr<Identifiable> id4 = std::make_shared<Identifiable>(4);
			std::shared_ptr<Identifiable> id1 = std::make_shared<Identifiable>(1);

			collection.add(id2);
			collection.add(id4);
			collection.add(id1);
			
			collection.sort();
			std::shared_ptr<Identifiable> actual = collection.find_by_id(1);
			Assert::IsTrue(actual == 1);
		}

		TEST_METHOD(find4)
		{
			Collection::IdentifiableCollection<Identifiable> collection;

			std::shared_ptr<Identifiable> id2 = std::make_shared<Identifiable>(2);
			std::shared_ptr<Identifiable> id4 = std::make_shared<Identifiable>(4);
			std::shared_ptr<Identifiable> id1 = std::make_shared<Identifiable>(1);

			collection.add(id2);
			collection.add(id4);
			collection.add(id1);

			collection.sort();
			std::shared_ptr<Identifiable> actual = collection.find_by_id(5);
			Assert::IsTrue(actual == nullptr);
		}

	};
}
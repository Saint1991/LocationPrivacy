#include "stdafx.h"
#include "CppUnitTest.h"
#include "IdentifiableCollection.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilsTest
{

	//Identifiableの派生クラスでもうまくいくか試すため
	class DerivedFromIdentifiable : public Identifiable<long> {
	public :
		DerivedFromIdentifiable(const long& id) : Identifiable<long>(id) {}
	};


	TEST_CLASS(IdentifiableCollectionTest)
	{
	public:

		TEST_METHOD(IdentifiableCollection_Constructor)
		{
			Collection::IdentifiableCollection<long, Identifiable<long>> collection; 
			Collection::IdentifiableCollection<long, DerivedFromIdentifiable> collection2;
		}

		//とりあえず各メソッド実行してみて，コンパイルが通っているかチェック
		TEST_METHOD(IdentifiableCollection_CompileCheck) 
		{
			Collection::IdentifiableCollection<long, Identifiable<long>> collection;
			collection.add(Identifiable<long>(1L));
			collection.add(std::make_shared<Identifiable<long>>(2L));
			collection.read_by_id(1L);
			collection.get_by_id(1L);
			collection.remove_by_id(1L);
			collection.contains(2L);
			collection.contains(Identifiable<long>(2L));
			collection.contains(std::make_shared<Identifiable<long>>(2L));
			collection.foreach([](std::shared_ptr<Identifiable<long>> element) {

			});
			collection.foreach([](std::shared_ptr<Identifiable<long> const> element) {

			});
		}
		
		TEST_METHOD(IdentifiableCollection_read_by_id)
		{
			Collection::IdentifiableCollection<long, Identifiable<long>> collection;
			collection.add(Identifiable<long>(1L));
			std::shared_ptr<Identifiable<long> const> id = collection.read_by_id(1L);
			Assert::AreEqual(1L, id->get_id());
		}
		/*
		TEST_METHOD(IdentifiableCollection_)
		{

		}

		TEST_METHOD(IdentifiableCollection_)
		{

		}

		TEST_METHOD(IdentifiableCollection_)
		{

		}

		TEST_METHOD(IdentifiableCollection_)
		{

		}

		TEST_METHOD(IdentifiableCollection_)
		{

		}

		TEST_METHOD(IdentifiableCollection_)
		{

		}

		TEST_METHOD(IdentifiableCollection_)
		{

		}

		TEST_METHOD(IdentifiableCollection_)
		{

		}

		TEST_METHOD(IdentifiableCollection_)
		{

		}*/

	};
}

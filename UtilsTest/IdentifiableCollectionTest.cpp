#include "stdafx.h"
#include "CppUnitTest.h"
#include "IdentifiableCollection.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilsTest
{

	//IdentifiableÇÃîhê∂ÉNÉâÉXÇ≈Ç‡Ç§Ç‹Ç≠Ç¢Ç≠Ç©ééÇ∑ÇΩÇﬂ
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
		
		TEST_METHOD(IdentifiableCollection_read_by_id)
		{
			Collection::IdentifiableCollection<long, Identifiable<long>> collection;
			//collection.add(Identifiable<long>(1L));
			//std::shared_ptr<Identifiable<long> const> id = collection.read_by_id(1L);

			//Assert::AreEqual(1L, id->get_id());
		
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

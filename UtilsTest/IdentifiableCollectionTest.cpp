#include "stdafx.h"
#include "CppUnitTest.h"
#include "IdentifiableCollection.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Test
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
	};
}

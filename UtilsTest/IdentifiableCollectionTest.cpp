#include "stdafx.h"
#include "CppUnitTest.h"
#include "IdentifiableCollection.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Test
{
	TEST_CLASS(IdentifiableCollectionTest)
	{
	public:

		TEST_METHOD(IdentifiableCollection_Constructor)
		{
			Collection::IdentifiableCollection<Identifiable> collection;
		}
	};
}

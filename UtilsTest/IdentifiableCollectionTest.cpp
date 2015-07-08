#pragma once
#include "stdafx.h"
#include "CppUnitTest.h"
#include "IdentifiableCollection.h"
#include "Identifiable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilsTest
{
	TEST_CLASS(IdentifiableCollectionTest)
	{
	public:
		
		TEST_METHOD(assertion_check)
		{
			Collection::IdentifiableCollection<Identifiable> collection;
		}

	};
}
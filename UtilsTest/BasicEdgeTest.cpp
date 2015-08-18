#include "stdafx.h"
#include "CppUnitTest.h"
#include "BasicEdge.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Graph;

namespace UtilsTest
{
	TEST_CLASS(BasicEdgeTest)
	{
	public:

		TEST_METHOD(BasicEdge_Constructor)
		{
			BasicEdge edge1(3);
			BasicEdge edge2(2);

			node_id to = edge1.get_to();
			Assert::AreEqual(3UL, edge1.get_to());
			Assert::AreEqual(2UL, edge2.get_to());
		}

		TEST_METHOD(BasicEdge_operators1) 
		{
			BasicEdge edge1(3);
			BasicEdge edge2(2);

			node_id to = edge1.get_to();
			Assert::IsFalse(edge1 == edge2);
			Assert::IsTrue(edge1 != edge2);
		}

		TEST_METHOD(BasicEdge_operators2)
		{
			BasicEdge edge1(3);
			BasicEdge edge2(3);

			node_id to = edge1.get_to();
			Assert::IsTrue(edge1 == edge2);
			Assert::IsFalse(edge1 != edge2);
		}
	};
}

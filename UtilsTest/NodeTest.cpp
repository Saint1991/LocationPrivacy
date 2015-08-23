#include "stdafx.h"
#include "CppUnitTest.h"
#include "Node.h"
#include "Edge.h"
#include "BasicPath.h"
#include "BasicPathData.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Graph;

namespace UtilsTest
{
	TEST_CLASS(NodeTest)
	{
	public:

		TEST_METHOD(Node_Constructor)
		{
			Node<double, Edge<BasicPathData>> node(2L, std::make_shared<double>(2.3));
			Assert::AreEqual(2.3, *node.data);
			Assert::AreEqual(2L, node.get_id());

			Node<double, BasicPath> node2(2L, std::make_shared<double>(2.3));
			Assert::AreEqual(2.3, *node2.data);
			Assert::AreEqual(2L, node2.get_id());
		}
	};
}

#include "stdafx.h"
#include "CppUnitTest.h"
#include "IdentifiableCollection.h"
#include "Node.h"
#include "Edge.h"
#include "BasicPathData.h"
#include "LatLng.h"
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
		const double ACCURACY = 1.0E-10;

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
			collection.get_id_list();
			collection.contains(2L);
			collection.contains(Identifiable<long>(2L));
			collection.contains(std::make_shared<Identifiable<long>>(2L));
			collection.foreach([](std::shared_ptr<Identifiable<long>> element) {

			});
			collection.foreach([](std::shared_ptr<Identifiable<long> const> element) {

			});

			Collection::IdentifiableCollection<Graph::node_id, Graph::Node<Geography::LatLng, Graph::Edge<Graph::BasicPathData>>> collection2;
			Assert::IsTrue(collection2.add(Graph::Node<Geography::LatLng, Graph::Edge<Graph::BasicPathData>>(1L, std::make_shared<Geography::LatLng>(10.0, 10.0))));
			Assert::IsTrue(collection2.add(std::make_shared<Graph::Node<Geography::LatLng, Graph::Edge<Graph::BasicPathData>>>(2L, std::make_shared<Geography::LatLng>(10.0, 20.0))));
			Assert::IsTrue(collection2.read_by_id(1L) != nullptr);
			Assert::IsTrue(collection2.get_by_id(1L) != nullptr);
			Assert::IsTrue(collection2.remove_by_id(1L));
			std::unique_ptr<std::vector<Graph::node_id>> id_list = collection2.get_id_list();
			Assert::AreEqual(1U, collection2.get_id_list()->size());
			Assert::AreEqual(1U, collection2.size());
			Assert::IsTrue(collection2.contains(2L));
			Assert::IsTrue(collection2.contains(Graph::Node<Geography::LatLng, Graph::Edge<Graph::BasicPathData> > (2L, std::make_shared<Geography::LatLng>(10.0, 20.0))));
			Assert::IsTrue(collection2.contains(std::make_shared<Graph::Node<Geography::LatLng, Graph::Edge<Graph::BasicPathData>>>(2L, std::make_shared<Geography::LatLng>(10.0, 20.0))));
			
			collection2.foreach([](std::shared_ptr<Identifiable<long>> element) {

			});
			collection2.foreach([](std::shared_ptr<Identifiable<long> const> element) {

			});
		}
		
		TEST_METHOD(IdentifiableCollection_read_by_id)
		{
			Collection::IdentifiableCollection<long, Identifiable<long>> collection;
			collection.add(Identifiable<long>(1L));
			std::shared_ptr<Identifiable<long> const> id = collection.read_by_id(1L);
		
			Assert::AreEqual(1L, id->get_id());
			Assert::IsTrue(nullptr == collection.read_by_id(2L));
		}
		
		TEST_METHOD(IdentifiableCollection_get_by_id)
		{

			Collection::IdentifiableCollection<std::string, Identifiable<std::string>> collection;
			collection.add(Identifiable<std::string>("C"));
			collection.add(Identifiable<std::string>("A"));
			collection.add(Identifiable<std::string>("B"));
			std::shared_ptr<Identifiable<std::string>> id = collection.get_by_id("A");
			Assert::AreEqual("A", id->get_id().c_str());

			*id = Identifiable<std::string>("D");
			
			int index = 0;
			collection.foreach([&index](std::shared_ptr<Identifiable<std::string>> id){
				index++;
				switch (index)
				{
				case 1:
					Assert::AreEqual("D", id->get_id().c_str());
					break;
				case 2:
					Assert::AreEqual("B", id->get_id().c_str());
					break;
				case 3:
					Assert::AreEqual("C", id->get_id().c_str());
					break;
				}
			});
		}

		TEST_METHOD(IdentifiableCollection_read_by_id2)
		{
			Collection::IdentifiableCollection<Graph::node_id, Graph::Node<Geography::LatLng, Graph::Edge<Graph::BasicPathData>>> collection;
			collection.add(Graph::Node < Geography::LatLng, Graph::Edge<Graph::BasicPathData>>(1L, std::make_shared<Geography::LatLng>(10.0, 20.0)));
			std::shared_ptr<Graph::Node<Geography::LatLng, Graph::Edge<Graph::BasicPathData>> const> node = collection.read_by_id(1L);
			Assert::AreEqual(1L, node->get_id());
			Assert::AreEqual(10.0, node->data->lat(), ACCURACY);
			Assert::AreEqual(20.0, node->data->lng(), ACCURACY);
		}
		
		TEST_METHOD(IdentifiableCollection_remove_by_id)
		{
			Collection::IdentifiableCollection<Graph::node_id, Graph::Node<Geography::LatLng, Graph::Edge<Graph::BasicPathData>>> collection;
			collection.add(Graph::Node<Geography::LatLng, Graph::Edge<Graph::BasicPathData>>(1L,std::make_shared<Geography::LatLng>(10.0, 20.0)));
			Assert::IsFalse(collection.remove_by_id(2L));
			Assert::AreEqual(1U,collection.size());

			Assert::IsTrue(collection.remove_by_id(1L));
			Assert::AreEqual(0U, collection.size());

			std::shared_ptr<Graph::Node<Geography::LatLng, Graph::Edge<Graph::BasicPathData>> const> node = collection.read_by_id(1L);
			Assert::IsTrue(nullptr == node);
		}
		
		TEST_METHOD(IdentifiableCollection_get_id_list)
		{
			Collection::IdentifiableCollection<Graph::node_id, Graph::Node<Geography::LatLng, Graph::Edge<Graph::BasicPathData>>> collection;
			collection.add(Graph::Node<Geography::LatLng, Graph::Edge<Graph::BasicPathData>>(1L, std::make_shared<Geography::LatLng>(10.0, 20.0)));
			collection.add(Graph::Node<Geography::LatLng, Graph::Edge<Graph::BasicPathData>>(2L, std::make_shared<Geography::LatLng>(15.0, 20.0)));
			const std::unique_ptr<std::vector<Graph::node_id>> list = collection.get_id_list();
			auto iter = list->begin();

			Assert::AreEqual(1L, *iter);
			iter++;
			Assert::AreEqual(2L, *iter);
			
		}
		
		TEST_METHOD(IdentifiableCollection_contains)
		{
			Collection::IdentifiableCollection<Graph::node_id, Graph::Node<Geography::LatLng, Graph::Edge<Graph::BasicPathData>>> collection;
			collection.add(Graph::Node<Geography::LatLng, Graph::Edge<Graph::BasicPathData>>(1L, std::make_shared<Geography::LatLng>(10.0, 20.0)));
			collection.add(Graph::Node<Geography::LatLng, Graph::Edge<Graph::BasicPathData>>(2L, std::make_shared<Geography::LatLng>(15.0, 20.0)));
			collection.add(Graph::Node<Geography::LatLng, Graph::Edge<Graph::BasicPathData>>(3L, std::make_shared<Geography::LatLng>(20.0, 20.0)));
			std::shared_ptr<Graph::Node<Geography::LatLng, Graph::Edge<Graph::BasicPathData>>> node1 = std::make_shared<Graph::Node<Geography::LatLng, Graph::Edge<Graph::BasicPathData>>>(1L, std::make_shared<Geography::LatLng>(10.0, 20.0));
			std::shared_ptr<Graph::Node<Geography::LatLng, Graph::Edge<Graph::BasicPathData>>> node2 = std::make_shared<Graph::Node<Geography::LatLng, Graph::Edge<Graph::BasicPathData>>>(2L, std::make_shared<Geography::LatLng>(15.0, 20.0));

			Assert::IsTrue(collection.contains(1L));
			Assert::IsTrue(collection.contains(2L));
			Assert::IsTrue(collection.contains(3L));
			Assert::IsFalse(collection.contains(4L));

			Assert::IsTrue(collection.contains(Graph::Node<Geography::LatLng, Graph::Edge<Graph::BasicPathData>>(1L, std::make_shared<Geography::LatLng>(10.0, 20.0))));
			Assert::IsFalse(collection.contains(Graph::Node<Geography::LatLng, Graph::Edge<Graph::BasicPathData>>(4L, std::make_shared<Geography::LatLng>(15.0, 20.0))));

			Assert::IsTrue(collection.contains(std::make_shared<Graph::Node<Geography::LatLng, Graph::Edge<Graph::BasicPathData>>>(1L, std::make_shared<Geography::LatLng>(10.0, 20.0))));
			Assert::IsFalse(collection.contains(std::make_shared<Graph::Node<Geography::LatLng, Graph::Edge<Graph::BasicPathData>>>(4L, std::make_shared<Geography::LatLng>(15.0, 20.0))));

		}
		
		TEST_METHOD(IdentifiableCollection_add)
		{
			Collection::IdentifiableCollection<Graph::node_id, Graph::Node<Geography::LatLng, Graph::Edge<Graph::BasicPathData>>> collection;
			collection.add(Graph::Node<Geography::LatLng, Graph::Edge<Graph::BasicPathData>>(1L, std::make_shared<Geography::LatLng>(10.0, 20.0)));
			
			try
			{
				collection.add(Graph::Node<Geography::LatLng, Graph::Edge<Graph::BasicPathData>>(1L, std::make_shared<Geography::LatLng>(15.0, 20.0)));
				Assert::Fail();
			}
			catch (DuplicatedIdException<Graph::node_id>)
			{

			}
			catch (const std::exception) {
				Assert::Fail();
			}

			Assert::IsTrue(collection.contains(1L));
			
		}

		TEST_METHOD(IdentifiableCollection_foreach)
		{
			Collection::IdentifiableCollection<Graph::node_id, Graph::Node<Geography::LatLng, Graph::Edge<Graph::BasicPathData>>> collection;
			collection.add(Graph::Node<Geography::LatLng, Graph::Edge<Graph::BasicPathData>>(1L, std::make_shared<Geography::LatLng>(10.0, 20.0)));
			collection.add(Graph::Node<Geography::LatLng, Graph::Edge<Graph::BasicPathData>>(2L, std::make_shared<Geography::LatLng>(20.0, 20.0)));
			collection.add(Graph::Node<Geography::LatLng, Graph::Edge<Graph::BasicPathData>>(3L, std::make_shared<Geography::LatLng>(30.0, 20.0)));
			
			int index = 0;
			collection.foreach([&index](std::shared_ptr<Graph::Node<Geography::LatLng, Graph::Edge<Graph::BasicPathData>> const> node) {
				index++;
				switch (index)
				{
				case 1:
					Assert::AreEqual(1L, node->get_id());
					break;
				case 2:
					Assert::AreEqual(2L, node->get_id());
					break;
				case 3:
					Assert::AreEqual(3L, node->get_id());
					break;
				}
			});
		}

	
	};
}

#include "stdafx.h"
#include "CppUnitTest.h"
#include "ObservedTrajectoryStructure.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Test
{

	TEST_CLASS(ObservedTrajectoryStructureTest)
	{
	public:

		std::shared_ptr<Observer::ObservedTrajectoryStructure> create_sample_structure()
		{
			std::shared_ptr<Observer::ObservedTrajectoryStructure> structure = std::make_shared<Observer::ObservedTrajectoryStructure>();
			Observer::ObservedTrajectoryStructure::base_iterator iter = structure->root<Observer::ObservedTrajectoryStructure::base_iterator>();
			iter->count_up();
			iter->count_up();
			iter->count_up();

			iter.add_child(std::make_shared<Observer::ObservedTrajectoryNode>(1, 1, std::make_shared<Graph::MapNodeIndicator>(1, Graph::NodeType::POI)));
			iter.add_child(std::make_shared<Observer::ObservedTrajectoryNode>(2, 1, std::make_shared<Graph::MapNodeIndicator>(2, Graph::NodeType::POI)));
			iter.add_child(std::make_shared<Observer::ObservedTrajectoryNode>(3, 1, std::make_shared<Graph::MapNodeIndicator>(3, Graph::NodeType::POI)));
			iter.add_child(std::make_shared<Observer::ObservedTrajectoryNode>(4, 1, std::make_shared<Graph::MapNodeIndicator>(4, Graph::NodeType::POI)));

			iter = structure->get_iter_by_id<Observer::ObservedTrajectoryStructure::base_iterator>(1);
			iter.add_child(std::make_shared<Observer::ObservedTrajectoryNode>(5, 2, std::make_shared<Graph::MapNodeIndicator>(5, Graph::NodeType::POI)));
			iter = structure->get_iter_by_id<Observer::ObservedTrajectoryStructure::base_iterator>(2);
			iter->connect_to(std::make_shared<Graph::BasicEdge>(5));
			iter = structure->get_iter_by_id<Observer::ObservedTrajectoryStructure::base_iterator>(3);
			iter.add_child(std::make_shared<Observer::ObservedTrajectoryNode>(6, 2, std::make_shared<Graph::MapNodeIndicator>(6, Graph::NodeType::POI)));
			iter = structure->get_iter_by_id<Observer::ObservedTrajectoryStructure::base_iterator>(4);
			iter.add_child(std::make_shared<Observer::ObservedTrajectoryNode>(7, 2, std::make_shared<Graph::MapNodeIndicator>(7, Graph::NodeType::POI)));

			iter = structure->get_iter_by_id<Observer::ObservedTrajectoryStructure::base_iterator>(5);
			iter->count_up();
			iter.add_child(std::make_shared<Observer::ObservedTrajectoryNode>(8, 3, std::make_shared<Graph::MapNodeIndicator>(8, Graph::NodeType::POI)));
			iter.add_child(std::make_shared<Observer::ObservedTrajectoryNode>(9, 3, std::make_shared<Graph::MapNodeIndicator>(9, Graph::NodeType::POI)));
			iter = structure->get_iter_by_id<Observer::ObservedTrajectoryStructure::base_iterator>(6);
			iter->connect_to(std::make_shared<Graph::BasicEdge>(9));
			iter = structure->get_iter_by_id<Observer::ObservedTrajectoryStructure::base_iterator>(7);
			iter->connect_to(std::make_shared<Graph::BasicEdge>(9));

			iter = structure->get_iter_by_id<Observer::ObservedTrajectoryStructure::base_iterator>(8);
			iter.add_child(std::make_shared<Observer::ObservedTrajectoryNode>(10, 4, std::make_shared<Graph::MapNodeIndicator>(10, Graph::NodeType::POI)));
			iter = structure->get_iter_by_id<Observer::ObservedTrajectoryStructure::base_iterator>(9);
			iter->count_up();
			iter->count_up();
			iter->connect_to(std::make_shared<Graph::BasicEdge>(10));
			iter.add_child(std::make_shared<Observer::ObservedTrajectoryNode>(11, 4, std::make_shared<Graph::MapNodeIndicator>(11, Graph::NodeType::POI)));
			iter.add_child(std::make_shared<Observer::ObservedTrajectoryNode>(12, 4, std::make_shared<Graph::MapNodeIndicator>(12, Graph::NodeType::POI)));

			iter = structure->get_iter_by_id<Observer::ObservedTrajectoryStructure::base_iterator>(10);
			iter->count_up();

			return structure;
		}

		TEST_METHOD(ObservedTrajectoryStructure_trajectory_length)
		{
			std::shared_ptr<Observer::ObservedTrajectoryStructure const> structure = create_sample_structure();
			size_t actual = structure->trajectory_length();
			Assert::AreEqual(4u, actual);
		}

		
		TEST_METHOD(ObservedTrajectoryStructure_find_node_id)
		{
			std::shared_ptr<Observer::ObservedTrajectoryStructure const> structure = create_sample_structure();
			Graph::node_id id = structure->find_node_id(Graph::MapNodeIndicator(6, Graph::NodeType::POI), 1);
			Assert::AreEqual(6L, id);
			id = structure->find_node_id(Graph::MapNodeIndicator(6, Graph::NodeType::POI), 2);
			Assert::AreEqual(-1L, id);
		}
		
	};
}

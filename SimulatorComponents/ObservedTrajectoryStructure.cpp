#include "stdafx.h"
#include "ObservedTrajectoryStructure.h"

namespace Observer
{

	///<summary>
	/// コンストラクタ
	///</summary>
	ObservedTrajectoryStructure::ObservedTrajectoryStructure() : Graph::Tree<ObservedTrajectoryNode, Graph::MapNodeIndicator, Graph::BasicEdge>()
	{
		initialize(std::make_shared<ObservedTrajectoryNode>(0, 0, nullptr));
	}


	///<summary>
	/// デストラクタ
	///</summary>
	ObservedTrajectoryStructure::~ObservedTrajectoryStructure()
	{
	}


	///<summary>
	/// 指定した値でroot_nodeを取得する
	///</summary>
	void ObservedTrajectoryStructure::initialize(std::shared_ptr<ObservedTrajectoryNode> root_node)
	{
		if (root_node->get_depth() != 0) throw std::invalid_argument("Depth of root node must be 0");
		Graph::Tree<ObservedTrajectoryNode, Graph::MapNodeIndicator, Graph::BasicEdge>::initialize(root_node);
	}


	///<summary>
	/// MapNodeIndicatorとPhaseを基にノードを探索し，見つかった場合はそのノードを指すイテレータを返す．
	/// 見つからない場合はnullptrを指すイテレータを返す．
	///</summary>
	ObservedTrajectoryStructure::base_iterator ObservedTrajectoryStructure::find_node(const Graph::MapNodeIndicator& id, int phase)
	{
		int depth = phase + 1;
		Collection::IdentifiableCollection<Graph::node_id, Observer::ObservedTrajectoryNode>::iterator iter = std::find_if(node_collection->begin(), node_collection->end(), [id, depth](std::shared_ptr<Identifiable<Graph::node_id>> element) {
			std::shared_ptr<Observer::ObservedTrajectoryNode> node = std::dynamic_pointer_cast<Observer::ObservedTrajectoryNode>(element);
			return node != nullptr && *node->data == id && depth == node->get_depth();
		});

		return iter == node_collection->end() ? ObservedTrajectoryStructure::base_iterator(-1, nullptr) : ObservedTrajectoryStructure::base_iterator((*iter)->get_id(), node_collection);
	}


	///<summary>
	/// マップのノードIDと訪問フェーズから対応するTrajectoryNodeのIDを取得する
	/// 該当するノードがない場合-1を返す
	///</summary>
	Graph::node_id ObservedTrajectoryStructure::find_node_id(const Graph::MapNodeIndicator& id, int phase) const
	{
		int depth = phase + 1;
		Collection::IdentifiableCollection<Graph::node_id, Observer::ObservedTrajectoryNode>::iterator iter = std::find_if(node_collection->begin(), node_collection->end(), [id, depth](std::shared_ptr<Identifiable<Graph::node_id>> element) {
			std::shared_ptr<Observer::ObservedTrajectoryNode> node = std::dynamic_pointer_cast<Observer::ObservedTrajectoryNode>(element);
			return node != nullptr && *node->data == id && depth == node->get_depth();
		});

		return iter == node_collection->end() ? -1 : (*iter)->get_id();
	}
}


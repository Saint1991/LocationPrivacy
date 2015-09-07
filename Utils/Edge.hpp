
namespace Graph 
{
	///<summary>
	/// コンストラクタ
	///</summary>
	///<param name='to'>接続先のノードID</param>
	///<param name='data'>エッジの持つ属性値</param>
	template <typename EDGE_DATA>
	Edge<EDGE_DATA>::Edge(const Graph::node_id& to, std::shared_ptr<EDGE_DATA> data) : BasicEdge(to), data(data)
	{
	}

	
	///<summary>
	/// コピーコンストラクタ
	///</summary>
	template <typename EDGE_DATA>
	Edge<EDGE_DATA>::Edge(const Edge<EDGE_DATA>& edge) :BasicEdge(edge._to)
	{
		EDGE_DATA copy = *edge.data;
		data = std::make_shared<EDGE_DATA>(copy);
	}

	
	///<summary>
	/// デストラクタ
	///</summary>
	template <typename EDGE_DATA>
	Edge<EDGE_DATA>::~Edge()
	{
	}

	///<summary>
	/// データの更新を行う
	///</summary>
	template <typename EDGE_DATA>
	void Edge<EDGE_DATA>::update_data(std::shared_ptr<EDGE_DATA> data)
	{
		this->data = data;
	}


	///<summary>
	/// 変更可能な状態でデータのポインタを取得する
	///</summary>
	template <typename EDGE_DATA>
	std::shared_ptr<EDGE_DATA> Edge<EDGE_DATA>::get_data()
	{
		return data;
	}

	///<summary>
	/// 変更不可の状態でデータのポインタを取得する
	///</summary>
	template <typename EDGE_DATA>
	std::shared_ptr<EDGE_DATA const> Edge<EDGE_DATA>::get_static_data() const
	{
		return data;
	}

	template <typename EDGE_DATA>
	bool Edge<EDGE_DATA>::operator==(const Edge<EDGE_DATA>& e) const
	{
		return _to == e.get_to();
	}

	template <typename EDGE_DATA>
	bool Edge<EDGE_DATA>::operator!=(const Edge<EDGE_DATA>& e) const
	{
		return _to != e.get_to();
	}
}


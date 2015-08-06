
///<summary>
/// コンストラクタ
///</summary>
template <typename NODEDATA, typename EDGEDATA>
Geography::GeoNode<NODEDATA, EDGEDATA>::GeoNode(Graph::node_id id, NODEDATA data) : Graph::Node<NODEDATA, EDGEDATA>(id, std::make_shared<NODEDATA>(data))
{
}


///<summary>
/// デストラクタ
///</summary>
template <typename NODEDATA, typename EDGEDATA>
Geography::GeoNode<NODEDATA, EDGEDATA>::~GeoNode()
{
}

template <typename NODEDATA, typename EDGEDATA>
double Geography::distance(const Geography::GeoNode<NODEDATA, EDGEDATA>& node1, const Geography::GeoNode<NODEDATA, EDGEDATA>& node2)
{
	std::shared_ptr<Geography::BasicGeoNodeData const> data1 = node1.data;
	std::shared_ptr<Geography::BasicGeoNodeData const> data2 = node2.data;
	double dist = Geography::distance(data1->get_position(), data2->get_position());
	return dist;
}

template <typename NODEDATA, typename EDGEDATA>
double Geography::azimuth_angle(const Geography::GeoNode<NODEDATA, EDGEDATA>& from, const Geography::GeoNode<NODEDATA, EDGEDATA>& to)
{
	std::shared_ptr<Geography::BasicGeoNodeData const> from_data = from.data;
	std::shared_ptr<Geography::BasicGeoNodeData const> to_data = to.data;
	double angle = Geography::azimuth_angle(from_data->get_position(), to_data->get_position());
	return angle;
}

namespace Geography
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename POI_DATA>
	Poi<POI_DATA>::Poi(std::string id, std::shared_ptr<POI_DATA> data) : id(id), data(data), connection_map(std::make_unique<std::unordered_map<node_poi_relation>>())
	{
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename POI_DATA>
	Poi<POI_DATA>::~Poi()
	{
	}


	///<summary>
	/// POI��ID�̎擾
	///</summary>
	template <typename POI_DATA>
	const std::string Poi<POI_DATA>::get_id() const
	{
		return id;
	}


	///<summary>
	/// �ύX�s�̏�Ԃŕێ����Ă���f�[�^���擾����
	///</summary>
	template <typename POI_DATA>
	std::shared_ptr<POI_DATA const> Poi<POI_DATA>::get_static_data() const
	{
		return data;
	}


	///<summary>
	/// �ێ����Ă���f�[�^���擾����
	/// �ύX�����g�ɂ����f�����̂Œ���
	///</summary>
	template <typename POI_DATA>
	std::shared_ptr<POI_DATA> Poi<POI_DATA>::get_data()
	{
		return data;
	}


	///<summary>
	/// �m�[�h�Ƃ̊֌W�\���p
	/// ((�m�[�hA��ID, �m�[�hB��ID), (�m�[�hA�܂ł̋���, �m�[�hB�܂ł̋���))��\���f�[�^���i�[����
	///</summary>
	template <typename POI_DATA>
	void Poi<POI_DATA>::add_relation(const node_poi_relation& relation)
	{
		connection_map->insert(relation);
	}


	///<summary>
	/// ����POI�ɗאڂ���m�[�h��ID�ꗗ���擾����
	///</summary>
	template <typaname POI_DATA>
	std::list<Graph::node_id> Poi<POI_DATA>::get_srounding_nodes() const
	{
		std::list<Graph::node_id> ret;
		for (std::unordered_map<node_poi_relation>::const_iterator iter = connection_map->begin(); iter != connection_map->end(); iter++) {
			std::pair<Graph::node_id, Graph::node_id> id_pair = iter->first;
			ret.push_back(id_pair.first);
			ret.push_back(id_pair.second);
		}

		ret.sort();
		ret.unique();
		return std::move(ret);
	}
}



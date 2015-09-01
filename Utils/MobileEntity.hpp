
namespace Entity
{

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename POSITION_TYPE, typename STATE_TYPE>
	MobileEntity<POSITION_TYPE, STATE_TYPE>::MobileEntity(entity_id id) : Identifiable<entity_id>(id), past_states(std::make_shared<std::vector<std::shared_ptr<STATE_TYPE>>>())
	{
	}


	///<summary>
	/// デストラクタ
	///</summary>
	template <typename POSITION_TYPE, typename STATE_TYPE>
	MobileEntity<POSITION_TYPE, STATE_TYPE>::~MobileEntity()
	{
	}


	///<summary>
	/// 与えられたstateを保存する
	///</summary>
	template <typename POSITION_TYPE, typename STATE_TYPE>
	void MobileEntity<POSITION_TYPE, STATE_TYPE>::save_state(std::shared_ptr<STATE_TYPE> state)
	{
		past_states->push_back(state);
	}

	///<summary>
	/// 現在位置を返します
	/// 存在しない場合はnullptrが返ります
	///</summary>
	template <typename POSITION_TYPE, typename STATE_TYPE>
	const std::shared_ptr<POSITION_TYPE const> MobileEntity<POSITION_TYPE, STATE_TYPE>::current_position() const
	{
		if (past_states->size() == 0) return nullptr;
		return past_states->back()->read_position();
	}


	///<summary>
	/// 過去countステップ前からの位置を返す
	///</summary>
	template <typename POSITION_TYPE, typename STATE_TYPE>
	const std::vector<std::shared_ptr<POSITION_TYPE const>> MobileEntity<POSITION_TYPE, STATE_TYPE>::previous_positions(unsigned int count) const
	{
		std::vector<std::shared_ptr<POSITION_TYPE const>> ret(count);

		unsigned int index = 0;
		for (std::vector<std::shared_ptr<STATE_TYPE const>>::const_iterator iter = past_states->end() - count; iter != past_states->end(); iter++) {
			ret.at(index++) = iter->read_position();
		}

		return ret;
	}
}


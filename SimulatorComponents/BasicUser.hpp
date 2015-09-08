
namespace Entity
{


	///<summary>
	/// コンストラクタ
	/// ユーザの動きは入力としてコンストラクタに渡す
	///</summary>
	template <typename STATE_TYPE>
	BasicUser<STATE_TYPE>::BasicUser(std::vector<std::shared_ptr<STATE_TYPE>> user_movement, entity_id id)
		: MobileEntity<Geography::LatLng, STATE_TYPE>(id), past_states(std::make_shared<std::vector<std::shared_ptr<STATE_TYPE>>>(user_movement))
	{
	}


	///<summary>
	/// デストラクタ
	///</summary>
	template <typename STATE_TYPE>
	BasicUser<STATE_TYPE>::~BasicUser()
	{
	}

}



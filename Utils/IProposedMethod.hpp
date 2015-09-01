
namespace Framework
{

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename MAP_TYPE, typename USER_TYPE, typename DUMMY_TYPE, typename REQUIREMENT_TYPE>
	IProposedMethod<MAP_TYPE, USER_TYPE, DUMMY_TYPE, REQUIREMENT_TYPE>::IProposedMethod(std::shared_ptr<MAP_TYPE const> map, std::shared_ptr<USER_TYPE const> user, std::shared_ptr<REQUIREMENT_TYPE const> requirement)
		: map(map), user(user), requirement(requirement)
	{
		size_t dummy_num = this->requirement->dummy_num;
		dummies = std::make_unique<std::vector<DUMMY_TYPE>>(dummy_num);
	}


	///<summary>
	/// デストラクタ
	///</summary>
	template <typename MAP_TYPE, typename USER_TYPE, typename DUMMY_TYPE, typename REQUIREMENT_TYPE>
	IProposedMethod<MAP_TYPE, USER_TYPE, DUMMY_TYPE, REQUIREMENT_TYPE>::~IProposedMethod()
	{
	}
}


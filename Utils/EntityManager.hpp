
namespace Entity
{


	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	EntityManager<DUMMY, USER, POSITION_TYPE>::EntityManager(std::shared_ptr<USER> user, int num_of_dummy, std::shared_ptr<Time::TimeSlotManager> timeslot)
		: timeslot(timeslot), dummies(std::make_shared<std::vector<std::shared_ptr<DUMMY>>>(num_of_dummy)), user(user)
	{
		for (int id = 1; id <= num_of_dummy; id++) {
			dummies->at(id - 1) = std::make_shared<DUMMY>(id, timeslot);
		}
	}


	///<summary>
	/// デストラクタ
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	EntityManager<DUMMY, USER, POSITION_TYPE>::~EntityManager()
	{
	}


	///<summary>
	/// ダミーを生成する
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	entity_id EntityManager<DUMMY, USER, POSITION_TYPE>::create_dummy()
	{
		entity_id new_id = dummies->size() + 1;
		dummies->push_back(std::make_shared<DUMMY>(new_id, timeslot));
		return new_id;
	}


	///<summary>
	/// 2エンティティ間の共有地点を設定する
	/// これを呼ぶたびに各ダミーの交差回数が+1されるので注意
	/// 交差設定に複数点の設定が必要な場合は一度だけこれを呼び出して，あとはset_positionの方をつかうこと
	/// ダミーIDはID=1からダミー数分の連番になっているのを想定している
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	void EntityManager<DUMMY, USER, POSITION_TYPE>::set_crossing_point_of_phase(entity_id id1, entity_id id2, POSITION_TYPE position1, POSITION_TYPE position2, int phase)
	{

		if (id1 < 1 || id2 < 1 || dummies->size() <= id1 || dummies->size() <= id2) {
			throw std::invalid_argument("INVALID dummy ID");
			return;
		}

		std::shared_ptr<DUMMY> dummy1 = dummies->at(id1 - 1);
		std::shared_ptr<DUMMY> dummy2 = dummies->at(id2 - 1);
		if (dummy1 != nullptr && dummy2 != nullptr && dummy1->get_id() == id1 && dummy2->get_id() == id2) {
			dummy1->set_crossing_position_of_phase(phase, position1);
			dummy2->set_crossing_position_of_phase(phase, position2);
		}
	}

	///<summary>
	/// 2エンティティ間の共有地点を設定する
	/// これを呼ぶたびに各ダミーの交差回数が+1されるので注意
	/// 交差設定に複数点の設定が必要な場合は一度だけこれを呼び出して，あとはset_positionの方をつかうこと
	/// ダミーIDはID=1からダミー数分の連番になっているのを想定している
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	void EntityManager<DUMMY, USER, POSITION_TYPE>::set_crossing_point_at(entity_id id1, entity_id id2, POSITION_TYPE position1, POSITION_TYPE position2, time_t time)
	{
		int phase = timeslot->find_phase_of_time(time);
		if (phase != INVALID) {
			set_crossing_point_of_phase(id1, id2, position1, position2, phase);
		}
	}


	///<summary>
	/// ダミーに地点の設定をする
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	void EntityManager<DUMMY, USER, POSITION_TYPE>::set_point_of_phase(entity_id id, POSITION_TYPE position, int phase)
	{
		if (id < 1 || dummies->size() <= id) {
			throw std::invalid_argument("INVALID dummy ID");
			return;
		}

		std::shared_ptr<DUMMY> dummy = dummies->at(id - 1);
		if (dummy != nullptr && dummy->get_id() == id) {
			dummy->set_position_of_phase(phase, position);
		}
	}

	///<summary>
	/// ダミーに地点の設定をする
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	void EntityManager<DUMMY, USER, POSITION_TYPE>::set_point_at(entity_id id, POSITION_TYPE type, time_t time)
	{
		int phase = timeslot->find_phase_of_time(time);
		if (phase != INVALID) {
			set_point_of_phase(id, type, phase);
		}
	}


	///<summary>
	/// ユーザを読み専用で取得
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	std::shared_ptr<USER const> EntityManager<DUMMY, USER, POSITION_TYPE>::get_user() const
	{
		return user;
	}


	///<summary>
	/// 指定したIDのダミーを取得
	/// 存在しない場合はnullptrが返る
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	std::shared_ptr<DUMMY> EntityManager<DUMMY, USER, POSITION_TYPE>::get_dummy_by_id(entity_id id)
	{
		std::shared_ptr<DUMMY> dummy = dummies->at(id - 1);
		if (dummy->get_id() == id) {
			return dummy;
		}
		return nullptr;
	}


	///<summary>
	/// 条件に合うダミーを取得
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	std::shared_ptr<DUMMY> EntityManager<DUMMY, USER, POSITION_TYPE>::find_dummy_if(const std::function<bool(std::shared_ptr<DUMMY const>)>& compare)
	{
		for (std::vector<std::shared_ptr<DUMMY>>::const_iterator iter = dummies->begin(); iter != dummies->end(); iter++) {
			if (compare(*iter)) return *iter;
		}
		return nullptr;
	}


	///<summary>
	/// 条件に合うダミーを全て取得する
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	std::vector<std::shared_ptr<DUMMY>> EntityManager<DUMMY, USER, POSITION_TYPE>::find_all_dummies_if(const std::function<bool(std::shared_ptr<DUMMY const>)>& compare)
	{
		std::vector<std::shared_ptr<DUMMY>> ret;
		for (std::vector<std::shared_ptr<DUMMY>>::const_iterator iter = dummies->begin(); iter != dummies->end(); iter++) {
			if (compare(*iter)) ret.push_back(*iter);
		}
		return ret;
	}


	///<summary>
	/// 指定したIDのダミーを取得
	/// 存在しない場合はnullptrが返る
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	std::shared_ptr<DUMMY const> EntityManager<DUMMY, USER, POSITION_TYPE>::read_dummy_by_id(entity_id id) const
	{
		std::shared_ptr<DUMMY const> dummy = dummies->at(id - 1);
		if (dummy->get_id() == id) {
			return dummy;
		}
		return nullptr;
	}


	///<summary>
	/// 交差回数が最小のエンティティのIDを取得
	/// 0の場合はユーザ，1以上ならダミー
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	entity_id EntityManager<DUMMY, USER, POSITION_TYPE>::get_min_cross_entity_id() const
	{
		entity_id ret = user->get_id();
		int min = user->get_cross_count();
		for (std::vector<std::shared_ptr<DUMMY>>::const_iterator iter = dummies->begin(); iter != dummies->end(); iter++) {
			if ((*iter)->get_cross_count() < min) {
				ret = (*iter)->get_id();
			}
		}
		return ret;
	}


	///<summary>
	/// 指定したPhaseにおける位置確定済みエンティティの平均位置を取得する
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	std::shared_ptr<POSITION_TYPE const> EntityManager<DUMMY, USER, POSITION_TYPE>::get_average_position_of_phase(int phase)
	{
		double x = 0.0;
		double y = 0.0;

		std::shared_ptr<POSITION_TYPE const> position = user->read_position_of_phase(phase);
		x += position->x();
		y += position->y();
		int fixed_count = 1;

		for (std::vector<std::shared_ptr<DUMMY>>::const_iterator iter = dummies->begin(); iter != dummies->end(); iter++) {
			position = (*iter)->read_position_of_phase(phase);
			if (position != nullptr) {
				fixed_count++;
				x += position->x();
				y += position->y();
			}
		}

		x /= fixed_count;
		y /= fixed_count;
		return std::move(std::make_shared<POSITION_TYPE const>(x, y));
	}


	///<summary>
	/// 時刻timeにおいて位置が確定しているエンティティの平均位置を取得します
	/// 不正な時刻の場合はnullptrを返します．
	///</summary>
	template <typename DUMMY, typename USER, typename POSITION_TYPE>
	std::shared_ptr<POSITION_TYPE const> EntityManager<DUMMY, USER, POSITION_TYPE>::get_average_position_at(time_t time)
	{
		int phase = timeslot->find_phase_of_time(time);
		if (phase != INVALID) {
			return get_average_position_of_phase(phase);
		}
		return nullptr;
	}
}



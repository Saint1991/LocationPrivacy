
namespace Entity
{


	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE, typename DUMMY, typename USER>
	EntityManager<POSITION_TYPE, TRAJECTORY_TYPE, DUMMY, USER>::EntityManager(std::shared_ptr<USER> user, int num_of_dummy, std::shared_ptr<Time::TimeSlotManager const> timeslot)
		: timeslot(timeslot), dummies(std::make_shared<std::vector<std::shared_ptr<DUMMY>>>(num_of_dummy)), user(user)
	{
		for (int id = 1; id <= num_of_dummy; id++) {
			dummies->at(id - 1) = std::make_shared<DUMMY>(id, timeslot);
		}
	}


	///<summary>
	/// デストラクタ
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE, typename DUMMY, typename USER>
	EntityManager<POSITION_TYPE, TRAJECTORY_TYPE, DUMMY, USER>::~EntityManager()
	{
	}


	///<summary>
	/// TimeSlotManagerを読み専用で取得
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE, typename DUMMY, typename USER>
	std::shared_ptr <Time::TimeSlotManager const> EntityManager<POSITION_TYPE, TRAJECTORY_TYPE, DUMMY, USER>::read_timeslot() const
	{
		return timeslot;
	}

	///<summary>
	/// ダミーを生成する
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE, typename DUMMY, typename USER>
	entity_id EntityManager<POSITION_TYPE, TRAJECTORY_TYPE, DUMMY, USER>::create_dummy()
	{
		entity_id new_id = dummies->size() + 1;
		dummies->push_back(std::make_shared<DUMMY>(new_id, timeslot));
		return new_id;
	}

	///<summary>
	/// ユーザを取得
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE, typename DUMMY, typename USER>
	std::shared_ptr<USER> EntityManager<POSITION_TYPE, TRAJECTORY_TYPE, DUMMY, USER>::get_user()
	{
		return user;
	}


	///<summary>
	/// ユーザ，ダミーの区別なくidに対応するエンティティを返す
	/// ただし存在しないidの場合はnullptrを返す
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE, typename DUMMY, typename USER>
	std::shared_ptr<Entity::MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>> EntityManager<POSITION_TYPE, TRAJECTORY_TYPE, DUMMY, USER>::get_entity_by_id(entity_id id)
	{
		return id == 0 ? user : get_dummy_by_id(id);
	}


	///<summary>
	/// 指定したIDのダミーを取得
	/// 存在しない場合はnullptrが返る
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE, typename DUMMY, typename USER>
	std::shared_ptr<DUMMY> EntityManager<POSITION_TYPE, TRAJECTORY_TYPE, DUMMY, USER>::get_dummy_by_id(entity_id id)
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
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE, typename DUMMY, typename USER>
	std::shared_ptr<DUMMY> EntityManager<POSITION_TYPE, TRAJECTORY_TYPE, DUMMY, USER>::find_dummy_if(const std::function<bool(std::shared_ptr<DUMMY const>)>& compare)
	{
		for (std::vector<std::shared_ptr<DUMMY>>::const_iterator iter = dummies->begin(); iter != dummies->end(); iter++) {
			if (compare(*iter)) return *iter;
		}
		return nullptr;
	}


	///<summary>
	/// 条件に合うダミーを全て取得する
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE, typename DUMMY, typename USER>
	std::vector<std::shared_ptr<DUMMY>> EntityManager<POSITION_TYPE, TRAJECTORY_TYPE, DUMMY, USER>::find_all_dummies_if(const std::function<bool(std::shared_ptr<DUMMY const>)>& compare)
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
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE, typename DUMMY, typename USER>
	std::shared_ptr<DUMMY const> EntityManager<POSITION_TYPE, TRAJECTORY_TYPE, DUMMY, USER>::read_dummy_by_id(entity_id id) const
	{
		std::shared_ptr<DUMMY const> dummy = dummies->at(id - 1);
		if (dummy->get_id() == id) {
			return dummy;
		}
		return nullptr;
	}
	

	///<summary>
	/// 読み専用で指定したエンティティを取得する
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE, typename DUMMY, typename USER>
	std::shared_ptr<MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE> const> EntityManager<POSITION_TYPE, TRAJECTORY_TYPE, DUMMY, USER>::read_entity_by_id(entity_id id) const
	{
		std::shared_ptr<MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE> const> entity = id == 0 ? user : read_dummy_by_id(id);
		return entity;
	}
		
	///<summary>
	/// 交差回数の小さい順にエンティティIDを格納したリストを返します
	/// 未生成のダミーも交差回数0として含まれるので注意
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE, typename DUMMY, typename USER>
	std::vector<std::pair<entity_id, int>> EntityManager<POSITION_TYPE, TRAJECTORY_TYPE, DUMMY, USER>::get_entity_id_list_order_by_cross_count(Entity::entity_id id_smaller_than) const
	{
		std::vector<std::pair<entity_id, int>> ret;
		ret.push_back(std::make_pair(0U, user->get_cross_count()));

		for (std::vector<std::shared_ptr<DUMMY>>::const_iterator iter = dummies->begin(); iter != dummies->end(); iter++) {
			if (*iter == nullptr) continue;
			entity_id id = (*iter)->get_id();
			if (id >= id_smaller_than) continue;
			int cross_count = (*iter)->get_cross_count();
			ret.push_back(std::make_pair(id, cross_count));
		}

		std::sort(ret.begin(), ret.end(), [](const std::pair<entity_id, int>& pair1, const std::pair<entity_id, int>& pair2) {
			return pair1.second < pair2.second || (pair1.second == pair2.second && pair1.first < pair2.first);
		});
		return ret;
	}


	///<summary>
	/// IDがidのエンティティと共有地点が設定されているエンティティのID集合を返す．
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE, typename DUMMY, typename USER>
	std::vector<Entity::entity_id> EntityManager<POSITION_TYPE, TRAJECTORY_TYPE, DUMMY, USER>::get_entities_cross_with(entity_id id) const
	{

		std::vector<Entity::entity_id> ret;

		// 指定されたIDがユーザのものの場合
		if (id == 0) {
			for (int phase = 0; phase < timeslot->phase_count(); phase++) {
				if (user->is_cross_set_at_phase(phase)) {
					Graph::MapNodeIndicator user_pos = user->read_trajectory()->read_node_pos_info_of_phase(phase).first;
					for (std::vector<std::shared_ptr<DUMMY>>::const_iterator iter = dummies->begin(); iter != dummies->end(); iter++) {
						Graph::MapNodeIndicator dummy_pos = (*iter)->read_trajectory()->read_node_pos_info_of_phase(phase).first;
						if ((*iter)->is_cross_set_at_phase(phase) && user_pos == dummy_pos) ret.push_back((*iter)->get_id());
					}
				}
			}
		}

		// 指定されたIDがダミーのものである場合
		else {
			std::shared_ptr<DUMMY const> dummy = read_dummy_by_id(id);
			for (int phase = 0; phase < timeslot->phase_count(); phase++) {
				if (dummy->is_cross_set_at_phase(phase)) {
					Graph::MapNodeIndicator dummy_pos = dummy->read_trajectory()->read_node_pos_info_of_phase(phase).first;
					
					Graph::MapNodeIndicator user_pos = user->read_trajectory()->read_node_pos_info_of_phase(phase).first;
					if (dummy_pos == user_pos && user->is_cross_set_at_phase(phase)) ret.push_back(0);
					
					for (std::vector<std::shared_ptr<DUMMY>>::const_iterator iter = dummies->begin(); iter != dummies->end(); iter++) {
						entity_id cross_dummy_id = (*iter)->get_id();
						if (cross_dummy_id == id) continue;
						Graph::MapNodeIndicator dummy_pos2 = (*iter)->read_trajectory()->read_node_pos_info_of_phase(phase).first;
						if ((*iter)->is_cross_set_at_phase(phase) && dummy_pos == dummy_pos2) ret.push_back(cross_dummy_id);
					}
				}
			}
		}

		return ret;
	}

	///<summary>
	/// ダミーの総数を取得
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE, typename DUMMY, typename USER>
	size_t EntityManager<POSITION_TYPE, TRAJECTORY_TYPE, DUMMY, USER>::get_dummy_count() const
	{
		return dummies->size();
	}


	///<summary>
	/// phaseにおいてすでに共有地点を設定されているエンティティ数を取得する
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE, typename DUMMY, typename USER>
	size_t EntityManager<POSITION_TYPE, TRAJECTORY_TYPE, DUMMY, USER>::get_total_cross_count_of_phase(int phase) const
	{
		size_t ret = 0;
		if (user->is_cross_set_at_phase(phase)) ret++;
		for (std::vector<std::shared_ptr<DUMMY>>::const_iterator dummy = dummies->begin(); dummy != dummies->end(); dummy++) {
			if ((*dummy)->is_cross_set_at_phase(phase)) ret++;
		}
		return ret;
	}

	
	///<summary>
	/// 共有地点を設定されているエンティティ数が最小のphaseを取得する
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE, typename DUMMY, typename USER>
	int EntityManager<POSITION_TYPE, TRAJECTORY_TYPE, DUMMY, USER>::get_phase_with_min_total_cross_count() const
	{
		int min_phase = -1;
		int min_cross_count = INT_MAX;
		for (int phase = 0; phase < timeslot->phase_count(); phase++) {
			size_t cross_count = get_total_cross_count_of_phase(phase);
			if (cross_count < min_cross_count) {
				min_cross_count = cross_count;
				min_phase = phase;
			}
		}
		return min_phase;
	}

	///<summary>
	/// 各ダミーについてexecute_functionを実行
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE, typename DUMMY, typename USER>
	void EntityManager<POSITION_TYPE, TRAJECTORY_TYPE, DUMMY, USER>::for_each_dummy(const std::function<void(entity_id, std::shared_ptr<DUMMY>)>& execute_function)
	{
		for (std::vector<std::shared_ptr<DUMMY>>::iterator iter = dummies->begin(); iter != dummies->end(); iter++) {
			if (*iter != nullptr) execute_function((*iter)->get_id(), *iter);
		}
	}

	///<summary>
	/// 各ダミーについてexecute_functionを実行
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE, typename DUMMY, typename USER>
	void EntityManager<POSITION_TYPE, TRAJECTORY_TYPE, DUMMY, USER>::for_each_dummy(const std::function<void(entity_id, std::shared_ptr<DUMMY const>)>& execute_function) const
	{
		for (std::vector<std::shared_ptr<DUMMY>>::const_iterator iter = dummies->begin(); iter != dummies->end(); iter++) {
			if (*iter != nullptr) {
				std::shared_ptr<DUMMY const> dummy = *iter;
				execute_function(dummy->get_id(), dummy);
			}
		}
	}


	///<summary>
	/// phaseにおける経路確定済みエンティティの位置を全て取得する
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE, typename DUMMY, typename USER>
	std::vector<std::shared_ptr<POSITION_TYPE const>> EntityManager<POSITION_TYPE, TRAJECTORY_TYPE, DUMMY, USER>::get_all_fixed_positions_of_phase(int phase) const
	{
		std::vector<std::shared_ptr<POSITION_TYPE const>> ret(1, user->read_position_of_phase(phase));
		for (std::vector<std::shared_ptr<DUMMY>>::const_iterator dummy = dummies->begin(); dummy != dummies->end(); dummy++) {
			std::shared_ptr<POSITION_TYPE const> dummy_pos = (*dummy)->read_position_of_phase(phase);
			if (dummy_pos != nullptr) ret.push_back(dummy_pos);
		}
		return ret;
	}

	///<summary>
	/// phaseにおける経路確定済みエンティティからなる凸包領域の面積を計算します
	/// ただしエンティティが1つの場合は0を，2つの場合は距離を返します．
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE, typename DUMMY, typename USER>
	double EntityManager<POSITION_TYPE, TRAJECTORY_TYPE, DUMMY, USER>::calc_convex_hull_size_of_fixed_entities_of_phase(int phase) const
	{
		std::vector<std::shared_ptr<POSITION_TYPE const>> positions = get_all_fixed_positions_of_phase(phase);
		if (positions.size() <= 1) return 0.0;
		return positions.size() == 2 ? Geography::dist(*positions.at(0), *positions.at(1)) : Geography::GeoCalculation::calc_convex_hull_size(positions);
	}

	///<summary>
	/// 各Phaseについて全エンティティの位置を引数にして繰り返す
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE, typename DUMMY, typename USER>
	void EntityManager<POSITION_TYPE, TRAJECTORY_TYPE, DUMMY, USER>::positions_for_each_phase(const std::function<void(int, time_t, const std::vector<std::shared_ptr<POSITION_TYPE const>>&)>& execute_function) const
	{
		timeslot->for_each_time([&](time_t time, long duration, int phase) {
			std::vector<std::shared_ptr<POSITION_TYPE const>> positions(dummies->size() + 1);
			positions->at(0) = user->read_position_of_phase(phase);
			for (entity_id dummy_id = 1; dummy_id <= dummies->size(); dummy_id++) {
				std::shared_ptr<DUMMY const> dummy = read_dummy_by_id(dummy_id);
				positions->at(dummy_id) = dummy->read_position_of_phase(phase);
			}
			execute_function(phase, time, positions);
		});
	}

	///<summary>
	/// 指定したPhaseにおける位置確定済みエンティティの平均位置を取得する
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE, typename DUMMY, typename USER>
	std::shared_ptr<POSITION_TYPE const> EntityManager<POSITION_TYPE, TRAJECTORY_TYPE, DUMMY, USER>::get_average_position_of_phase(int phase) const
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
		
		std::shared_ptr<POSITION_TYPE const> ret = std::is_same<Graph::Coordinate, POSITION_TYPE>::value ? std::make_shared<POSITION_TYPE const>(x, y) : std::make_shared<POSITION_TYPE const>(y, x);
		return ret;
	}


	///<summary>
	/// 時刻timeにおいて位置が確定しているエンティティの平均位置を取得します
	/// 不正な時刻の場合はnullptrを返します．
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE, typename DUMMY, typename USER>
	std::shared_ptr<POSITION_TYPE const> EntityManager<POSITION_TYPE, TRAJECTORY_TYPE, DUMMY, USER>::get_average_position_at(time_t time) const
	{
		int phase = timeslot->find_phase_of_time(time);
		if (phase != INVALID) {
			return get_average_position_of_phase(phase);
		}
		return nullptr;
	}

	
	///<summary>
	/// 各phaseにおけるセルに存在するユーザおよび生成済みダミーの数を計算します
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE, typename DUMMY, typename USER>
	int EntityManager<POSITION_TYPE, TRAJECTORY_TYPE, DUMMY, USER>::get_entity_count_within_boundary(int phase, const Graph::Rectangle<POSITION_TYPE>& boundary) const 
	{
		
		int counter = 0;

		//ユーザが領域内に存在するか確認
		std::shared_ptr<POSITION_TYPE const> user_position = user->read_position_of_phase(phase);
		if (user_position != nullptr && boundary.contains(*user_position)) counter++;

		//領域内のダミー数の確認
		for_each_dummy([&boundary, &counter, phase](entity_id id, std::shared_ptr<DUMMY const> dummy) {
			std::shared_ptr<POSITION_TYPE const> dummy_position = dummy->read_position_of_phase(phase);
			if (dummy_position != nullptr && boundary.contains(*dummy_position)) counter++;
		});

		return counter;
	}

	///<summary>
	/// 各phaseにおけるセルに存在するユーザおよび生成済みダミーの数を計算します
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE, typename DUMMY, typename USER>
	int EntityManager<POSITION_TYPE, TRAJECTORY_TYPE, DUMMY, USER>::get_entity_count_within_boundary(int phase, double top, double left, double bottom, double right) const
	{
		return get_entity_count_within_boundary(phase, Graph::Rectangle<POSITION_TYPE>(top, left, bottom, right));
	}

	///<summary>
	/// 全てのエンティティの交差回数の合計を計算します
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE, typename DUMMY, typename USER>
	int EntityManager<POSITION_TYPE, TRAJECTORY_TYPE, DUMMY, USER>::get_all_entities_total_crossing_count() const
	{
		int counter = 0;
		
		counter += user->get_cross_count();

		for_each_dummy([&counter](Entity::entity_id id, std::shared_ptr<DUMMY const> dummy) {
			counter += dummy->get_cross_count();
		});
		
		return counter;
	}


	///<summary>
	/// from_phaseの位置からto_phaseまでの位置のユーザの進行方向の角度(rad)を返す．
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE, typename DUMMY, typename USER>
	double EntityManager<POSITION_TYPE, TRAJECTORY_TYPE, DUMMY, USER>::get_user_direction_of_phase(int from_phase, int to_phase) const
	{
		std::shared_ptr<Geography::LatLng const> from_position = user->read_position_of_phase(from_phase);
		std::shared_ptr<Geography::LatLng const> to_position = user->read_position_of_phase(to_phase);
		if (from_position == nullptr || to_position == nullptr) return DBL_MAX;
		double direction = Geography::GeoCalculation::lambert_azimuth_angle(*from_position, *to_position);
		return direction;
	}
}



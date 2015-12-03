#include "stdafx.h"
#include "MizunoMethod.h"

namespace Method
{


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	MizunoMethod::MizunoMethod(
		std::shared_ptr<Map::BasicDbMap const> map, 
		std::shared_ptr<User::BasicUser<Geography::LatLng>> user, 
		std::shared_ptr<User::PreferenceTree> observed_preference_tree,
		std::shared_ptr<Requirement::PreferenceRequirement const> requirement, 
		std::shared_ptr<Time::TimeSlotManager const> time_manager)
		: Framework::IProposedMethod<Map::BasicDbMap, User::BasicUser<Geography::LatLng>, Entity::Dummy<Geography::LatLng>, Requirement::PreferenceRequirement, Geography::LatLng, Graph::SemanticTrajectory<Geography::LatLng>>(map, user, requirement, time_manager),
		observed_preference_tree(observed_preference_tree), observed_preference_tree_copy(std::make_shared<User::PreferenceTree>(*observed_preference_tree)),
		setting_anonymous_areas(std::make_unique<std::vector<double>>(requirement->dummy_num))
	{
	}

	MizunoMethod::~MizunoMethod()
	{
	}

	//�ȉ��ݒ蓽���̈挈��̂��߂̊֐�
	//���`
	double linear(int dummy_id, size_t dummy_num, double required_anonymous_area)
	{
		double size = required_anonymous_area * dummy_id / dummy_num;
		return dummy_id = 1 ? std::sqrt(size) : size;
	}


	///<summary>
	/// ������
	/// �ݒ蓽���̈�̌���
	///</summary>
	void MizunoMethod::initialize()
	{
		set_setting_anonymous_area(linear);
	}


	///<summary>
	/// ��Ď�@�̓��e
	///</summary>
	void MizunoMethod::decide_dummy_positions()
	{
		std::shared_ptr<User::BasicUser<>> user = entities->get_user();
		int num_of_group_a_dummy = requirement->required_preference_conservation * requirement->dummy_num;
		int num_of_group_b_dummy = requirement->dummy_num - num_of_group_a_dummy;

		decide_dummy_positions_of_group_a(num_of_group_a_dummy);
		decide_dummy_positions_of_group_b(num_of_group_b_dummy);
	}

	#pragma region Setting Anonymous Area
	///<summary>
	/// �ݒ蓽���̈�̐���
	///</summary>
	void MizunoMethod::set_setting_anonymous_area(const std::function<double(int, size_t, double)>& generator)
	{
		for (int dummy_id = 1; dummy_id <= setting_anonymous_areas->size(); dummy_id++) {
			setting_anonymous_areas->at(dummy_id - 1) = generator(dummy_id, setting_anonymous_areas->size(), requirement->required_anonymous_area);
		}
	}

	

	#pragma endregion

	#pragma region GroupA
	
	///<summary>
	/// �O���[�vA�̃_�~�[�̐���
	///</summary>
	void MizunoMethod::decide_dummy_positions_of_group_a(int num_of_group_a_dummy)
	{
		for (int current_dummy_id = 1; current_dummy_id <= num_of_group_a_dummy; current_dummy_id++) {
			
			//���[�U�̚n�D�̖؂Ɋ܂܂��e�v���t�B�b�N�X�ɂ��ăJ�e�S���V�[�P���X�ƃX�R�A�̑g���v�Z����
			std::vector<sequence_score_set> sequence_scores = calc_sequence_score_set(current_dummy_id);

			//���ۂ̌o�H�̐���
			std::vector<trajectory_score_set> trajectory_scores = calc_trajectory_score_set(current_dummy_id, sequence_scores);
			
			//�o�H�������ł��Ȃ������ꍇ�͗�O�𓊂��ďI�� (�b��I���u)
			if (trajectory_scores.size() == 0) throw Framework::TrajectoryNotFoundException("Cannot found valid trajectory following preference tree");
			std::sort(trajectory_scores.begin(), trajectory_scores.end(), [](const trajectory_score_set& left, const trajectory_score_set& right) {
				return left.second > right.second;
			});

			//�X�R�A���ő�̂��̂̒����烉���_����1�I�o���C�o�H���m�肳����
			std::vector<std::pair<std::vector<Graph::MapNodeIndicator>, Entity::cross_target>> candidates;
			double max_score = trajectory_scores.begin()->second;
			for (std::vector<trajectory_score_set>::const_iterator iter = trajectory_scores.begin(); iter != trajectory_scores.end(); iter++) {
				if (iter->second == max_score) candidates.push_back(iter->first);
			}

			//�o�H�̑I��
			Math::Probability generator;
			int pick_index = generator.uniform_distribution(0, candidates.size() - 1);
			std::pair<std::vector<Graph::MapNodeIndicator>, Entity::cross_target> trajectory = candidates.at(pick_index);

			//���݌��蒆�̃G���e�B�e�B�̌o�H���m�肳����
			std::shared_ptr<Entity::Dummy<Geography::LatLng>> current_dummy = entities->get_dummy_by_id(current_dummy_id);
			std::vector<Graph::MapNodeIndicator> route = trajectory.first;
			int cross_phase = trajectory.second.first;
			for (int phase = 0; phase < route.size(); phase++) {
				Graph::MapNodeIndicator node_id = route.at(phase);
				std::shared_ptr<Map::BasicPoi const> visit_poi = map->get_static_poi(node_id.id());
				Geography::LatLng position(visit_poi->lat(), visit_poi->lng());
				phase == cross_phase ? current_dummy->set_crossing_position_of_phase(phase, node_id, position) : current_dummy->set_position_of_phase(phase, node_id, position);
				current_dummy->set_category_of_phase(phase, visit_poi->category_id());
				std::cout << visit_poi->category_name() << "\t";
			}
			std::cout << std::endl;

			//observed_preference_tree_copy�Ɍ���g���W�F�N�g���̌��ʂ𔽉f����
			observed_preference_tree_copy->add_sequence_counter(current_dummy->read_trajectory()->get_category_sequence());
		}
	}

	///<summary>
	/// �V�[�P���X�Ƌ��L�n�_�̌��̑g<S, SP>�ɑ΂���X�R�A�̌v�Z
	///</summary>
	std::vector<std::pair<std::pair<Collection::Sequence<User::category_id>, Entity::cross_target>, double>> MizunoMethod::calc_sequence_score_set(Entity::entity_id current_dummy_id)
	{
		std::shared_ptr<User::BasicUser<Geography::LatLng> const> user = entities->get_user();
		std::shared_ptr<User::PreferenceTree const> preference_tree = user->preference;
		std::shared_ptr<Graph::SemanticTrajectory<> const> user_trajectory  = user->read_trajectory();
		std::vector<sequence_score_set> ret;

		// ����̃g���W�F�N�g�����n�D�̖؂̍ő咷��蒷���ꍇ�C�v���t�B�b�N�X�̍ő咷�̒����̂��̂�ΏۂƂ���D
		size_t max_depth = preference_tree->max_depth();
		size_t usable_prefix_length = min(max_depth, user_trajectory->phase_count());

		for (int prefix_length = usable_prefix_length - 0; prefix_length <= usable_prefix_length; prefix_length++) {
			
			//�^�̚n�D�̖؂Ɋ܂܂�邷�ׂĂ�Prefix�ɂ���
			preference_tree->for_each_prefix(prefix_length, [&](const Collection::Sequence<User::category_id>& prefix, double sup_u) {
				
				//Score_{pref}�̌v�Z
				double sup_o = observed_preference_tree->get_support_of(prefix);
				double sup_o_cp = observed_preference_tree_copy->get_support_of(prefix);
				double score_pref = preference_based_score(sup_u, sup_o_cp);

				//Score_{cross}�̌v�Z
				for (int target_entity_id = 0; target_entity_id < current_dummy_id; target_entity_id++) {
					
					std::shared_ptr<Entity::Dummy<>> target_entity = target_entity_id == 0 ? entities->get_user() : entities->get_dummy_by_id(target_entity_id);
					std::shared_ptr<Graph::SemanticTrajectory<> const> trajectory = target_entity->read_trajectory();
					Collection::Sequence<Entity::category_id> target_sequence = trajectory->get_category_sequence();
					
					for (int target_phase = 0; target_phase < time_manager->phase_count(); target_phase++) {
						
						double score_cross = 0.0;
						int delta = target_sequence.subsequence(0, target_phase) == prefix.subsequence(0, target_phase) ? 1 : 0;
						Entity::cross_target cross = Entity::NOTHING; 
						if (delta != 0) {

							//�����̎����Ƒ���
							cross = std::make_pair(target_phase, target_entity_id);
							
							//��������̃T�|�[�g
							double sup_e_cp = observed_preference_tree_copy->get_support_of(target_sequence);

							//n_share(t)�̌v�Z
							int n_share = entities->get_total_cross_count_of_phase(target_phase);

							//�������ݒ肳��Ă���G���e�B�e�B�̃T�|�[�g�̍��v�l�̌v�Z
							double sup_sum = 0.0;
							std::vector<Entity::entity_id> cross_entities = entities->get_entities_cross_with(target_entity_id);
							for (std::vector<Entity::entity_id>::const_iterator target_entity_id = cross_entities.begin(); target_entity_id != cross_entities.end(); target_entity_id++) {
								Collection::Sequence<Entity::category_id> target_sequence = entities->read_dummy_by_id(*target_entity_id)->read_trajectory()->get_category_sequence(0, prefix_length - 1);
								sup_sum += observed_preference_tree->get_support_of(target_sequence);
							}
							// Score_Cross�̌v�Z
							score_cross = cross_based_score(delta, n_share, sup_o, sup_e_cp, sup_sum);
						}
						
						//���̂��̂̑Ή��͕ۗ��ň�U���̂��̂̂ݍ̗p
						double score = total_sequence_score(score_pref, score_cross);
						if (score >= 0) ret.push_back(std::make_pair(std::make_pair(prefix, cross), score));
					}
				}
			});
		}

		return ret;
	}


	///<summary>
	/// �V�[�P���X�ɑ΂���X�R�A
	///</summary>
	double MizunoMethod::total_sequence_score(double score_pref, double score_cross)
	{
		return B * score_pref + (1 - B) * score_cross;
	}

	///<summary>
	/// Score_pref�̌v�Z��
	///</summary>
	double MizunoMethod::preference_based_score(double sup_u, double sup_o)
	{
		return sup_u - sup_o;
	}


	///<summary>
	/// Score_cross�̌v�Z��
	///</summary>
	double MizunoMethod::cross_based_score(int delta, int n_share, double sup_o, double sup_e, double sup_sum)
	{
		return delta == 0 ? 0.0 : (1.0 / (1.0 + n_share)) * sup_o * sup_e / (1.0 + sup_sum);
	}


	
	///<summary>
	/// �v�Z�����J�e�S���V�[�P���X�Ɋ�Â��Ď��ۂ̈ړ��o�H�𐶐�����
	///</summary>
	std::vector<std::pair<std::pair<std::vector<Graph::MapNodeIndicator>, Entity::cross_target>, double>> MizunoMethod::calc_trajectory_score_set(Entity::entity_id current_dummy_id, const std::vector<sequence_score_set>& sequence_scores)
	{
		std::vector<trajectory_score_set> ret;
		std::vector<Graph::MapNodeIndicator> ret_trajectory(time_manager->phase_count(), Graph::MapNodeIndicator(INVALID, Graph::NodeType::INVALID));

		std::random_device rd;
		std::mt19937_64 generator(rd());

		//���ۂ̌o�H�̐���
		for (std::vector<sequence_score_set>::const_iterator iter = sequence_scores.begin(); iter != sequence_scores.end(); iter++) {
			
			double score = iter->second;
			Collection::Sequence<User::category_id> category_sequence = iter->first.first;
			Entity::cross_target cross = iter->first.second;
			if (category_sequence.size() < time_manager->phase_count()) {
				for (int i = 0; i < time_manager->phase_count() - category_sequence.size(); i++) category_sequence.push_back(User::ANY);
			}
			
			//�J�e�S���V�[�P���X�Ƌ��L�n�_������Ɋ�̓_�����߂�
			std::pair<int, Graph::MapNodeIndicator> basis = determine_point_basis(category_sequence, cross);

			//��̓_����Ƀg���W�F�N�g�������̚n�D���J��Ԃ�
			std::shared_ptr<std::vector<Graph::MapNodeIndicator>> trajectory = nullptr;
			for (int i = 0; i <= MAX_TRAJECTORY_CREATION_TRIAL; i++) {
				trajectory = create_trajectory(current_dummy_id, basis, category_sequence);
			}
			
			if (trajectory == nullptr) continue;
			
			//�����Ńg���W�F�N�g���ɑ΂��ăX�R�A���v�Z����ret�ɒǉ�����
		}

		return ret;
	}
	

	///<summary>
	/// �o�H���̈�_���߂�
	/// �������ݒ�ł��Ă����ꍇ�́C�ݒ肳�ꂽ���L�n�_���C����Ă��Ȃ��ꍇ�͎���0�̎��_�ɃO���b�h��p���ăG���e�B�e�B���ψ�ɂȂ�悤�Ƀ����_���Ȋ�_�����肷��
	///</summary>
	std::pair<int, Graph::MapNodeIndicator> MizunoMethod::determine_point_basis(const Collection::Sequence<User::category_id>& category_sequence, const Entity::cross_target& cross)
	{

		std::random_device rd;
		std::mt19937_64 generator(rd());

		//�����t�F�[�Y�ƌ�������
		Entity::entity_id cross_target = cross.second;
		
		int phase_basis = cross.first;
		Graph::MapNodeIndicator point_basis(INVALID, Graph::NodeType::INVALID);

		//���L�n�_���ݒ�ł��Ă����ꍇ
		if (cross != Entity::NOTHING) {
			point_basis = cross.second == 0 ? entities->get_user()->read_node_pos_info_of_phase(cross.first).first : entities->read_dummy_by_id(cross.second)->read_node_pos_info_of_phase(cross.first).first;
		}

		//���L�n�_���ݒ�ł��Ȃ������ꍇ
		//9�~9�̃O���b�h�̈�𐶐����C�G���e�B�e�B���ŏ��̒n�_�����_���肷��
		else {
			//phase 0�ɂ����錈��ς݃G���e�B�e�B�̈ʒu�̃��X�g
			phase_basis = 0;
			std::vector<std::shared_ptr<Geography::LatLng const>> fixed_positions = entities->get_all_fixed_positions_of_phase(phase_basis);

			//�O���b�h�̐���
			std::shared_ptr<Geography::LatLng const> center = entities->get_average_position_of_phase(phase_basis);
			int cell_num_on_side = 3;
			double grid_side_length = std::sqrt(requirement->required_anonymous_area);
			std::unique_ptr<Graph::Grid<Geography::LatLng>> grid = std::make_unique<Graph::Grid<Geography::LatLng>>(*center, cell_num_on_side, grid_side_length);

			//�O���b�h�ɒn�_��o�^���Z�����̓_�����Ȃ����ɕ��ׂ�
			//�������̏ꏊ�̕��т̓����_���ɂ���
			std::shared_ptr<Map::BasicPoi const> visit_target = nullptr;
			grid->add_points(fixed_positions);
			std::vector<std::pair<Graph::cell_id, size_t>> min_cells = grid->get_cell_ids_order_by_point_count();

			std::vector<std::pair<Graph::cell_id, size_t>>::iterator right;
			for (std::vector<std::pair<Graph::cell_id, size_t>>::iterator left = min_cells.begin(); left != min_cells.end(); left = right + 1) {
				right = std::min_element(left, min_cells.end(), [](const std::pair<Graph::cell_id, size_t >& left, const std::pair<Graph::cell_id, size_t>& min) {
					return left <= min;
				});
				std::shuffle(left, right + 1, generator);
			}

			//�v�f�������Ȃ��Z�����珇��POI��T������
			for (std::vector<std::pair<Graph::cell_id, size_t>>::const_iterator iter = min_cells.begin(); iter != min_cells.end(); iter++) {
				std::shared_ptr<Graph::Rectangle<Geography::LatLng> const> boundary = grid->read_cell_by_id(iter->first);
				User::category_id target_category = category_sequence.at(phase_basis);
				visit_target = map->find_random_poi_within_boundary(*boundary, target_category);
				if (visit_target != nullptr) {
					point_basis = Graph::MapNodeIndicator(visit_target->get_id(), Graph::NodeType::POI);
					break;
				}
			}

			//�Y������J�e�S���̓_��������Ȃ������ꍇ
			if (visit_target == nullptr) throw Framework::TrajectoryNotFoundException("Trajectory Can't Created");
		}

		return std::make_pair(phase_basis, point_basis);
	}

	#pragma endregion GroupA�̃_�~�[����
	
	///<summary>
	/// �O���[�vB�̃_�~�[�̐���
	///</summary>
	void MizunoMethod::decide_dummy_positions_of_group_b(int num_of_group_b_dummy)
	{
		std::random_device rd;
		std::mt19937_64 generator(rd());

		for (int current_dummy_id = requirement->dummy_num - num_of_group_b_dummy + 1; current_dummy_id <= requirement->dummy_num;  current_dummy_id++) {

			//��_�̌���
			//�ݒ�ς݂̋��L�n�_�����ŏ��̃G���e�B�e�B�������_���Ɉ�I�o
			std::vector<std::pair<Entity::entity_id, int>> entity_cross_counts = entities->get_entity_id_list_order_by_cross_count();
			std::vector<std::pair<Entity::entity_id, int>>::iterator last_min_entity_element = std::min_element(entity_cross_counts.begin(), entity_cross_counts.end(), [](const std::pair<Entity::entity_id, int>& compare, const std::pair<Entity::entity_id, int>& min) {
				return compare.first <= min.first;
			});
			std::shuffle(entity_cross_counts.begin(), ++last_min_entity_element, generator);
			Entity::entity_id min_entity = entity_cross_counts.begin()->first;
			std::shared_ptr<Entity::Dummy<>> target_entity = min_entity == 0 ? entities->get_user() : entities->get_dummy_by_id(min_entity);

			//�������ݒ�̎����̂������L�n�_���ݒ肳��Ă���G���e�B�e�B�����ŏ��̎����������_���ɑI��
			std::vector<std::pair<int, size_t>> cross_counts;
			for (int phase = 0; phase < time_manager->phase_count(); phase++) {
				if (target_entity->is_cross_set_at_phase(phase)) continue;
				size_t cross_count = entities->get_total_cross_count_of_phase(phase);
				cross_counts.push_back(std::make_pair(phase, cross_count));
			}
			std::sort(cross_counts.begin(), cross_counts.end(), [](const std::pair<int, size_t>& left, const std::pair<int, size_t>& right) {
				return left.second < right.second;
			});
			std::vector<std::pair<int, size_t>>::iterator last_min_phase_element = std::min_element(cross_counts.begin(), cross_counts.end(), [](const std::pair<int, size_t>& compare, const std::pair<int, size_t>& min) {
				return compare.second <= min.second;
			});
			std::shuffle(cross_counts.begin(), ++last_min_phase_element, generator);
			int target_phase = cross_counts.at(0).first;
			Graph::MapNodeIndicator point_basis = target_entity->read_node_pos_info_of_phase(target_phase).first;

			//��_����Ɍo�H������
			Collection::Sequence<User::category_id> any_category_sequence(time_manager->phase_count(), User::ANY);
			std::shared_ptr<std::vector<Graph::MapNodeIndicator>> trajectory = create_trajectory(current_dummy_id, std::make_pair(target_phase, point_basis), any_category_sequence);

			if (trajectory == nullptr) throw Framework::TrajectoryNotFoundException("Can't found reachable Trajectory");

			//�o�H�̓o�^
			int phase = 0;
			for (std::vector<Graph::MapNodeIndicator>::const_iterator iter = trajectory->begin(); iter != trajectory->end(); iter++, phase++) {
				std::shared_ptr<Map::BasicPoi const> visit_poi = map->get_static_poi(iter->id());
				std::shared_ptr<Geography::LatLng const> position = std::make_shared<Geography::LatLng const>(visit_poi->lat(), visit_poi->lng());
				User::category_id category = visit_poi->data->get_category_id();
				target_entity->set_category_of_phase(phase, category);
				target_entity->set_position_of_phase(phase, *iter, *position);
				if (phase == target_phase) target_entity->register_as_cross_position(phase);
			}
		}
	}


	///<summary>
	/// ��̓_����ɓ��B�\���𖞂����o�H���������s����
	/// �o�H�ɂ̓����_����������C�������B�\�Ȍo�H�������ł��Ȃ������ꍇ��nullptr��Ԃ�
	///</summary>
	std::shared_ptr<std::vector<Graph::MapNodeIndicator>> MizunoMethod::create_trajectory(Entity::entity_id current_dummy_id, const std::pair<int, Graph::MapNodeIndicator>& basis, const Collection::Sequence<User::category_id>& category_sequence)
	{
		//�����_�}�C�Y�֘A
		Math::Probability prob;
		std::random_device rd;
		std::mt19937_64 generator(rd());

		//�����Ƀg���W�F�N�g�����쐬����
		std::shared_ptr<std::vector<Graph::MapNodeIndicator>> ret = nullptr;

		//������Ԃ͌��肵����̓_
		int previous_phase = basis.first;
		std::shared_ptr<Map::BasicPoi const> previous_poi = map->get_static_poi(basis.second.id());
		Geography::LatLng previous_latlng(previous_poi->lat(), previous_poi->lng());

		//�O�������̌���
		for (int phase = previous_phase - 1; 0 <= phase; phase--) {

			//���K���z�ɂ��ƂÂ��ă_�~�[�̑��x�����肷��
			double current_dummy_speed = prob.gaussian_distribution(requirement->average_speed_of_dummy, requirement->speed_range_of_dummy);

			//�e�퐧��
			User::category_id target_category = category_sequence.at(phase);
			double setting_anonymous_area = setting_anonymous_areas->at(current_dummy_id);
			time_t interval = std::abs(time_manager->time_of_phase(phase) - time_manager->time_of_phase(previous_phase));

			//�O�̒n�_����Ƃ��Ĉ�ӂ̒��������B�\�ȋ�����2�{�ƂȂ�̈���v�Z���C�T���̈�Ƃ���
			double boundary_side_length = current_dummy_speed * interval * 2;
			double boundary_side_length_lng = 0.000009 * boundary_side_length;
			double boundary_side_length_lat = 0.000007 * boundary_side_length;
			double boundary_top = previous_latlng.lat() + boundary_side_length_lat / 2.0;
			double boundary_bottom = boundary_top - boundary_side_length_lat;
			double boundary_left = previous_latlng.lng() - boundary_side_length_lng / 2.0;
			double boundary_right = previous_latlng.lng() + boundary_side_length_lng;
			Graph::Rectangle<Geography::LatLng> search_boundary(boundary_top, boundary_left, boundary_bottom, boundary_right);

			//�T���̈����ɊY������J�e�S����POI��S�Ď擾����
			std::vector<std::shared_ptr<Map::BasicPoi const>> pois = map->find_pois_of_category_within_boundary(search_boundary, target_category);
			if (pois.size() == 0) throw Framework::TrajectoryNotFoundException("Any POIs with the corresponding category were not found");
			std::shuffle(pois.begin(), pois.end(), generator);

			double current_anonymous_area = entities->calc_convex_hull_size_of_fixed_entities_of_phase(phase) >= setting_anonymous_area;

			//���[�U�̐i�s����
			std::shared_ptr<User::BasicUser<Geography::LatLng> const> user = entities->get_user();
			double user_direction = Geography::GeoCalculation::lambert_azimuth_angle(*user->read_position_of_phase(previous_phase), *user->read_position_of_phase(phase));

			for (std::vector<std::shared_ptr<Map::BasicPoi const>>::const_iterator poi = pois.begin(); poi != pois.end(); poi++) {

				//���B�s�\�ȓ_�̓X�L�b�v
				if (!map->is_reachable(Graph::MapNodeIndicator(previous_poi->get_id(), Graph::NodeType::POI), Graph::MapNodeIndicator((*poi)->get_id(), Graph::NodeType::POI), current_dummy_speed, interval)) continue;

				//���ɐݒ蓽���̈��B���ł��Ă���ꍇ
				//���[�U�̐i�s�����ɓ�����
				if (current_anonymous_area >= setting_anonymous_area) {
					Geography::LatLng current_latlng = Geography::LatLng((*poi)->lat(), (*poi)->lng());
					double dummy_direction = Geography::GeoCalculation::lambert_azimuth_angle(previous_latlng, current_latlng);
					if (std::abs(user_direction - dummy_direction) <= THETA) {
						//ret_trajectory.at(phase) = Graph::MapNodeIndicator((*poi)->get_id(), Graph::NodeType::POI);
						previous_poi = *poi;
						previous_latlng = current_latlng;
						break;
					}
				}

				//�B���ł��Ă��Ȃ��ꍇ
				//�ݒ蓽���̈悪��������_����T��
				else {
					std::vector<std::shared_ptr<Geography::LatLng const>> fixed_positions = entities->get_all_fixed_positions_of_phase(phase);
					std::shared_ptr<Geography::LatLng const> current_latlng = std::make_shared<Geography::LatLng const>((*poi)->lat(), (*poi)->lng());
					fixed_positions.push_back(current_latlng);
					double anonymous_area = Geography::GeoCalculation::calc_convex_hull_size(fixed_positions);
					if (anonymous_area >= setting_anonymous_area) {

					}
				}
				previous_phase = phase;
			}





		}

		//�㔼�����̌���
		previous_poi = map->get_static_poi(basis.second.id());
		previous_phase = basis.first;
		for (int phase = previous_phase + 1; phase < time_manager->phase_count(); phase++) {

		}

	}
}


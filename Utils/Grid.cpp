#include "stdafx.h"
#include "Grid.h"

namespace Graph
{

	#pragma region Cell
	
	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename POSITION_TYPE>
	Cell<POSITION_TYPE>::Cell(cell_id id, const Rectangle<POSITION_TYPE>& boundary) : Identifiable<cell_id>(id), Rectangle<POSITION_TYPE>(boundary)
	{

	}


	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename POSITION_TYPE>
	Cell<POSITION_TYPE>::Cell(cell_id id, double top, double left, double right, double bottom) : Identifiable<cell_id>(id), Rectangle<POSITION_TYPE>(top, left, right, bottom)
	{
		
	}


	///<summary>
	/// デストラクタ
	///</summary>
	template <typename POSITION_TYPE>
	Cell<POSITION_TYPE>::~Cell()
	{

	}

	#pragma endregion

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename POSITION_TYPE>
	Grid<POSITION_TYPE>::Grid(const POSITION_TYPE& center_position, int cell_num_on_side, double side_length) 
		: cell_list(std::make_unique<Collection::IdentifiableCollection<cell_id, Cell<POSITION_TYPE>>>()),
		points(std::make_unique<std::vector<std::shared_ptr<POSITION_TYPE const>>>())
	{
		build_grid(center_position, cell_num_on_side, side_length);
	}


	///<summary>
	/// デストラクタ
	///</summary>
	template <typename POSITION_TYPE>
	Grid<POSITION_TYPE>::~Grid()
	{
	}


	///<summary>
	/// Coordinate版 グリッドの作成
	///</summary>
	template <>
	void Grid<Graph::Coordinate>::build_grid(const Graph::Coordinate& center_position, int cell_num_on_side, double side_length)
	{
		double grid_left = center_position.x() - side_length / 2.0;
		double grid_top = center_position.y() + side_length / 2.0;

		double cell_side_length = side_length / cell_num_on_side;
		
		for (int y = 0; y < cell_num_on_side; y++) {
			for (int x = 0; x < cell_num_on_side; x++) {
				cell_id id = cell_num_on_side * y + x;
				double cell_left = grid_left + cell_side_length  * x;
				double cell_top = grid_top - cell_side_length * y;
				Rectangle<Coordinate> cell_area(cell_top, cell_left, cell_top - cell_side_length, cell_left + cell_side_length);
				cell_list->add(std::make_shared<Cell<Coordinate>>(id, cell_area));
			}
		}
	}

	///<summary>
	/// LatLng版 グリッドの作成
	///</summary>
	template <>
	void Grid<Geography::LatLng>::build_grid(const Geography::LatLng& center_position, int cell_num_on_side, double side_length)
	{
		Geography::LatLng top_left = Geography::GeoCalculation::calc_translated_point(center_position, side_length / 2.0, 3.0 * M_PI / 2.0);
		top_left.translate(side_length / 2.0, M_PI);
		
		double cell_side_length = side_length / cell_num_on_side;

		Geography::LatLng point_basis;
		for (int y = 0; y < cell_num_on_side; y++) {
			point_basis = top_left;
			point_basis.translate(cell_num_on_side * y, M_PI_2);
			for (int x = 0; x < cell_num_on_side; x++) {
				cell_id id = cell_num_on_side * y + x;
				Geography::LatLng cell_top_left = point_basis;
				point_basis.translate(cell_side_length, 0);
				Geography::LatLng cell_bottom_right = Geography::GeoCalculation::calc_translated_point(point_basis, cell_side_length, M_PI_2);
				Rectangle<Geography::LatLng> cell_area(cell_top_left.lat(), cell_top_left.lng(), cell_bottom_right.lat(), cell_bottom_right.lng());
				cell_list->add(std::make_shared<Cell<Geography::LatLng>>(id, cell_area));
			}
		}
	}

	///<summary>
	/// 地点を追加する
	///</summary>
	template <typename POSITION_TYPE>
	void Grid<POSITION_TYPE>::add_point(std::shared_ptr<POSITION_TYPE const> point)
	{
		if (point != nullptr) points->push_back(point);
	}

	///<summary>
	/// 地点を追加する
	///</summary>
	template <typename POSITION_TYPE>
	void Grid<POSITION_TYPE>::add_point(const POSITION_TYPE& point)
	{
		points->push_back(std::make_shared<POSITION_TYPE const>(point));
	}


	///<summary>
	/// 地点集合を全て追加する
	///</summary>
	template <typename POSITION_TYPE>
	void Grid<POSITION_TYPE>::add_points(const std::vector<std::shared_ptr<POSITION_TYPE const>>& point_list)
	{
		for (std::vector<std::shared_ptr<POSITION_TYPE const>>::const_iterator iter = point_list.begin(); iter != point_list.end(); iter++) {
			if (*iter != nullptr) points->push_back(*iter);
		}
	}


	///<summary>
	/// 地点集合を全て追加する
	///</summary>
	template <typename POSITION_TYPE>
	void Grid<POSITION_TYPE>::add_points(const std::vector<POSITION_TYPE>& point_list)
	{
		for (std::vector<POSITION_TYPE>::const_iterator iter = point_list.begin(); iter != point_list.end(); iter++) {
			points->push_back(std::make_shared<POSITION_TYPE const>(*iter));
		}
	}


	///<summary>
	/// 各セルについてexecute_functionを実行する
	///</summary>
	template <typename POSITION_TYPE>
	void Grid<POSITION_TYPE>::for_each_cell(const std::function<void(std::shared_ptr<Cell<POSITION_TYPE> const>)>& execute_function) const
	{
		for (Collection::IdentifiableCollection<cell_id, Cell<POSITION_TYPE>>::const_iterator iter = cell_list->begin(); iter != cell_list->end(); iter++) {
			std::shared_ptr<Cell<POSITION_TYPE> const> target_cell = std::dynamic_pointer_cast<Cell<POSITION_TYPE> const>(*iter);
			execute_function(target_cell);
		}
	}


	///<summary>
	/// 指定したセル内に含まれる点の数を返す
	///</summary>
	template <typename POSITION_TYPE>
	size_t Grid<POSITION_TYPE>::get_point_count_within_cell(cell_id id) const
	{
		std::shared_ptr<Cell<POSITION_TYPE> const> target_cell = cell_list->read_by_id(id);
		if (target_cell == nullptr) throw std::invalid_argument("Cell Not Found");

		int count = 0;
		for (std::vector<std::shared_ptr<POSITION_TYPE const>>::const_iterator point = points->begin(); point != points->end(); point++) {
			if (target_cell->contains(**point)) count++;
		}
		return count;
	}


	///<summary>
	/// 各セルに含まれる点の数を返す
	///</summary>
	template <typename POSITION_TYPE>
	std::vector<std::pair<cell_id, size_t>> Grid<POSITION_TYPE>::get_point_count_of_all_cells() const
	{
		std::vector<std::pair<cell_id, size_t>> ret(cell_list->size());
		std::unique_ptr<std::vector<cell_id>> id_list = cell_list->get_id_list();
		for (std::vector<cell_id>::const_iterator id = id_list->begin(); id != id_list->end(); id++) {
			size_t point_count = get_point_count_within_cell(*id);
			ret.push_back(std::make_pair(*id, point_count));
		}
		return ret;
	}


	///<summary>
	/// 地点数が最小のセルIDのリストを返す
	///</summary>
	template <typename POSITION_TYPE>
	std::vector<cell_id> Grid<POSITION_TYPE>::get_all_cell_id_with_min_point_count() const
	{
		std::vector<cell_id> ret;

		std::vector<std::pair<cell_id, size_t>> cell_point_counts = get_point_count_of_all_cells();
		std::vector<std::pair<cell_id, size_t>>::const_iterator min_iter = std::min_element(cell_point_counts.begin(), cell_point_counts.end(), [](const std::pair<cell_id, size_t>& left, const std::pair<cell_id, size_t>& right) {
			return left.second < right.second;
		});

		size_t min_count = min_iter->second;
		for (std::vector<std::pair<cell_id, size_t>>::const_iterator iter = cell_point_counts.begin(); iter != cell_point_counts.end(); iter++) {
			if (iter->second == min_count) ret.push_back(iter->first);
		}
		return ret;
	}


	///<summary>
	/// idに基づきCellを取得する
	///</summary>
	template <typename POSITION_TYPE>
	std::shared_ptr<Cell<POSITION_TYPE> const> Grid<POSITION_TYPE>::read_cell_by_id(cell_id id) const
	{
		return cell_list->read_by_id(id);
	}
}


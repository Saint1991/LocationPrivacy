#ifdef UTILS_EXPORTS
#define GRID_API __declspec(dllexport)
#else
#define GRID_API __declspec(dllimport)
#endif
#pragma once
#include "Coordinate.h"
#include "LatLng.h"
#include "Rectangle.h"
#include "GeoCalculation.h"

namespace Graph
{

	typedef int cell_id;
	
	template <typename POSITION_TYPE = Geography::LatLng>
	class GRID_API Grid
	{
	public:
		typedef std::pair<cell_id, Rectangle<POSITION_TYPE>> cell;
		Grid(const POSITION_TYPE& center_position, int cell_num_on_side, double side_length);
		~Grid();

		void add_point(std::shared_ptr<POSITION_TYPE const> point);
		void add_point(const POSITION_TYPE& point);
		void add_points(const std::vector<std::shared_ptr<POSITION_TYPE const>>& points);
		void add_points(const std::vector<POSITION_TYPE>& points);

		void for_each_cell(const std::function<void(std::shared_ptr<cell const>)>& execute_function) const;
		size_t get_point_count_within_cell(cell_id id) const;
		std::vector<std::pair<cell_id, size_t>> get_point_count_of_all_cells() const;

	protected:
		std::unique_ptr<std::vector<std::shared_ptr<cell>>> cell_list;
		std::unique_ptr<std::vector<std::shared_ptr<POSITION_TYPE const>>> points;

	private:
		void build_grid(const POSITION_TYPE& center, int cell_num_on_side, double side_length);
	};

	//–¾Ž¦“I“ÁŽê‰»
	template class Grid<Graph::Coordinate>;
	template class Grid<Geography::LatLng>;
}



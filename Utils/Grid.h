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
#include "Identifiable.h"
#include "IdentifiableCollection.h"

namespace Graph
{

	#pragma region Cell
	typedef unsigned int cell_id;
	
	template <typename POSITION_TYPE = Geography::LatLng>
	struct GRID_API Cell : public Identifiable<cell_id>, public Rectangle<POSITION_TYPE>
	{
		Cell(cell_id id, const Rectangle<POSITION_TYPE>& boundary);
		Cell(cell_id id, double top, double left, double right, double bottom);
		~Cell();
	};

	//–¾Ž¦“I“ÁŽê‰»
	template struct Cell<Graph::Coordinate>;
	template struct Cell<Geography::LatLng>;
	#pragma region Cell‚Ì’è‹`

	#pragma region Grid
	template <typename POSITION_TYPE = Geography::LatLng>
	class GRID_API Grid
	{
	public:
		Grid(const POSITION_TYPE& center_position, int cell_num_on_side, double side_length);
		~Grid();

		void add_point(std::shared_ptr<POSITION_TYPE const> point);
		void add_point(const POSITION_TYPE& point);
		void add_points(const std::vector<std::shared_ptr<POSITION_TYPE const>>& points);
		void add_points(const std::vector<POSITION_TYPE>& points);

		void for_each_cell(const std::function<void(std::shared_ptr<Cell<POSITION_TYPE> const>)>& execute_function) const;
		size_t get_point_count_within_cell(cell_id id) const;
		std::vector<std::pair<cell_id, size_t>> get_point_count_of_all_cells() const;

		std::vector<cell_id> get_all_cell_id_with_min_point_count() const;
		std::shared_ptr<Cell<POSITION_TYPE> const> read_cell_by_id(cell_id id) const;

	protected:
		std::unique_ptr<Collection::IdentifiableCollection<cell_id, Cell<POSITION_TYPE>>> cell_list;
		std::unique_ptr<std::vector<std::shared_ptr<POSITION_TYPE const>>> points;

	private:
		void build_grid(const POSITION_TYPE& center, int cell_num_on_side, double side_length);
	};
	#pragma endregion

	//–¾Ž¦“I“ÁŽê‰»
	//template class Grid<Graph::Coordinate>;
	template class Grid<Geography::LatLng>;
}



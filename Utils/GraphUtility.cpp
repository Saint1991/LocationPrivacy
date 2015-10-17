#include "stdafx.h"
#include "GraphUtility.h"
#include "Vector2d.h"


Graph::GraphUtility::GraphUtility()
{
}


Graph::GraphUtility::~GraphUtility()
{
}


///<summary>
/// �񃁃��o�֐�
/// cross_product��3�_�w��o�[�W����
///</summary>
double cross_p(const Graph::Coordinate& o, const Graph::Coordinate& a, const Graph::Coordinate& b)
{
	Graph::Vector2d v1 = a - o;
	Graph::Vector2d v2 = b - o;
	return Graph::cross_product(v1, v2);
}


///<summary>
/// �ʕ�ɊY��������W�̃��X�g���擾���܂��D
/// Monotone Chain Convex Hull
/// (�Q�� https://en.wikibooks.org/wiki/Algorithm_Implementation/Geometry/Convex_hull/Monotone_chain#C.2B.2B)
/// �n����vector��x���W�ŏ����Ƀ\�[�g�����̂Œ���
///</summary>
const std::vector<std::shared_ptr<Graph::Coordinate const>> Graph::GraphUtility::convex_hull(std::vector<std::shared_ptr<Graph::Coordinate const>>& point_list)
{

	//X���W�ŏ����Ƀ\�[�g
	std::sort(point_list.begin(), point_list.end(), [](const std::shared_ptr<Graph::Coordinate const>& a, const std::shared_ptr<Graph::Coordinate const>& b) -> bool {
		return a->x() < b->x() || (a->x() == b->x() && a->y() < b->y());
	});

	std::vector<std::shared_ptr<Graph::Coordinate const>> lower_list;
	std::unique_ptr<std::vector<std::shared_ptr<Graph::Coordinate const>>> upper_list;

	//Lower Hull�̌v�Z
	for (std::vector<std::shared_ptr<Graph::Coordinate const>>::const_iterator iter = point_list.begin(); iter != point_list.end(); iter++) {		
		while (lower_list.size() >= 2 && cross_p(*lower_list.at(lower_list.size() - 2), *lower_list.at(lower_list.size() - 1), **iter) <= 0) {
			lower_list.pop_back();
		}
		lower_list.push_back(*iter);
	}

	//Upper Hull�̌v�Z
	for (std::vector<std::shared_ptr<Graph::Coordinate const>>::const_reverse_iterator iter = point_list.rbegin(); iter != point_list.rend(); iter++) {
		while (upper_list->size() >= 2 && cross_p(*upper_list->at(upper_list->size() - 2), *upper_list->at(upper_list->size() - 1), **iter) <= 0) {
			upper_list->pop_back();
		}
		upper_list->push_back(*iter);
	}

	//�d���v�f�̍폜
	lower_list.pop_back();
	upper_list->pop_back();
	
	//Lower Hull��Upper Hull�̌���
	lower_list.insert(lower_list.end(), std::make_move_iterator(upper_list->begin()), std::make_move_iterator(upper_list->end()));

	return lower_list;
}


///<summary>
/// �^����ꂽCoordinate�̃��X�g����ʕ�̈�̖ʐς��v�Z����
/// (�Q�� https://ja.wikipedia.org/wiki/���p�`#.E9.9D.A2.E7.A9.8D.E5.85.AC.E5.BC.8F)
/// �n����vector��x���W�ŏ����Ƀ\�[�g�����̂Œ���
///</summary>
double Graph::GraphUtility::calc_convex_hull_size(std::vector<std::shared_ptr<Graph::Coordinate const>>& point_list)
{
	const std::vector<std::shared_ptr<Graph::Coordinate const>> convex_hull_points = convex_hull(point_list);
	
	double size = 0.0;
	for (unsigned int i = 0; i < convex_hull_points.size(); i++) {
		unsigned j = i + 1 == convex_hull_points.size() ? 0 : i + 1;
		size += Graph::cross_product(Graph::Vector2d(*convex_hull_points.at(i)), Graph::Vector2d(*convex_hull_points.at(j)));
 	}

	return 0.5 * std::abs(size);
}
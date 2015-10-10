#include "stdafx.h"
#include "Rectangle.h"

namespace Graph {

	///<summary>
	/// コンストラクタ
	///</summary>
	
	Rectangle::Rectangle(double top , double left , double bottom , double right)
	{

	}

	///<summary>
	/// デストラクタ
	///</summary>
	
	Rectangle::~Rectangle()
	{

	}

	///<summary>
	/// 正方形の高さを取得
	///</summary>
	
	double Rectangle::get_height()
	{
		return std::abs(top - bottom);
	}


	///<summary>
	/// 正方形の幅を取得
	///</summary>
	
	double Rectangle::get_width() {
		return right - left;
	}

	///<summary>
	/// 正方形の面積を取得
	///</summary>
	
	double Rectangle::get_area() {
		return get_height() * get_width();
	}


	///<summary>
	/// 領域内にエンティティがあるかどうかを取得
	/// 緯度経度Ver．
	///</summary>
	
	bool Rectangle::contains(const std::shared_ptr<Geography::LatLng const> position) const{
		if (bottom < position->y() && position->x() <= top && left <= position->x() && position->x() < right)
		{
			return true;
		}
		else
		{
			return false;
		}
		
	}

	///<summary>
	/// 領域内にエンティティがあるかどうかを取得
	/// xy座標Ver．
	///</summary>
	bool Rectangle::contains(const std::shared_ptr<Graph::Coordinate const> position) const {
		if (bottom < position->y() && position->x() <= top && left <= position->x() && position->x() < right)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

}

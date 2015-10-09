#include "stdafx.h"
#include "Rectangle.h"

namespace Graph {

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	
	Rectangle::Rectangle(double top , double left , double bottom , double right)
	{

	}

	///<summary>
	/// �f�X�g���N�^
	///</summary>
	
	Rectangle::~Rectangle()
	{

	}

	///<summary>
	/// �����`�̍������擾
	///</summary>
	
	double Rectangle::get_height()
	{
		return std::abs(top - bottom);
	}


	///<summary>
	/// �����`�̕����擾
	///</summary>
	
	double Rectangle::get_width() {
		return right - left;
	}

	///<summary>
	/// �����`�̖ʐς��擾
	///</summary>
	
	double Rectangle::get_area() {
		return get_height() * get_width();
	}


	///<summary>
	/// �̈���ɃG���e�B�e�B�����邩�ǂ������擾
	/// �ܓx�o�xVer�D
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
	/// �̈���ɃG���e�B�e�B�����邩�ǂ������擾
	/// xy���WVer�D
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

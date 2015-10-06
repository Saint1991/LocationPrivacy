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



}
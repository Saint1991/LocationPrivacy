#pragma once
#include "stdafx.h"

///<summary>
/// to_string()�����N���X�ł��邱�Ƃ��������߂̃C���^�t�F�[�X
///</summary>
class Serializable
{
public: 
	virtual const std::string to_string() const = 0;
};
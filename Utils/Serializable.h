#pragma once
#include "stdafx.h"

///<summary>
/// to_string()を持つクラスであることを示すためのインタフェース
///</summary>
class Serializable
{
public: 
	virtual const std::string to_string() const = 0;
};
#pragma once
#include "stdafx.h"

namespace IO 
{

	///<summary>
	/// “üo—Íü‚è‚Ì—áŠO‚ğˆµ‚¤
	///</summary>
	class IOException : public std::runtime_error
	{
	public:
		IOException(std::string& what_arg) : std::runtime_error(what_arg){}
		IOException(const char* what_arg) : std::runtime_error(what_arg) {}
	};
}

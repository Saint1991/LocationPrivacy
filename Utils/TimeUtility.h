#pragma once

namespace Time
{
	class TimeUtility
	{
	private:
		TimeUtility();
		~TimeUtility();

	public:
		static const std::string current_timestamp();
	};
}



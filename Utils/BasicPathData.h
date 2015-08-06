#pragma once
namespace Geography
{
	class BasicPathData
	{
	protected:
		double distance;

	public:
		const double get_distance() const;
		BasicPathData();
		~BasicPathData();
	};
}



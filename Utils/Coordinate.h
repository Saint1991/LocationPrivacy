#ifdef UTILS_EXPORTS
#define COORDINATE_API __declspec(dllexport)
#else
#define COORDINATE_API __declspec(dllimport)
#endif

#pragma once
#include "Identifiable.h"
#include "FileExportable.h"

namespace Graph 
{
	
	///<summary>
	/// ユークリッド空間の点を表すクラス
	///</summary>
	class  COORDINATE_API Coordinate : public Identifiable<unsigned int>
	{
	protected:
		double _x;
		double _y;
	
	public:
		static const std::string X;
		static const std::string Y;

		Coordinate(unsigned int id = 0);
		Coordinate(double x, double y, unsigned int id = 0);
		~Coordinate(); 

		double x() const;
		double y() const;
		
		std::unordered_map<std::string, std::string> get_export_data() const;

		COORDINATE_API friend Coordinate operator+(const Coordinate& c1, const Coordinate& c2);
		COORDINATE_API friend Coordinate operator-(const Coordinate& c1, const Coordinate& c2);
		COORDINATE_API friend double dist(const Coordinate& c1, const Coordinate& c2);
	};
}



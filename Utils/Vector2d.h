#ifdef UTILS_EXPORTS
#define VECTOR2D_API __declspec(dllexport)
#else
#define VECTOR2D_API __declspec(dllimport)
#endif

#pragma once
namespace Graph {
	
	///<summary>
	/// 2次元ベクタを表すクラス
	///</summary>
	class VECTOR2D_API Vector2d
	{
	private:
		double _x;
		double _y;
	
	public:	
		double x() const;
		double y() const;

		Vector2d(double x, double y);
		virtual ~Vector2d();

		double length() const;
		void rotate(double radian);
		void operator=(Vector2d v);
		VECTOR2D_API friend Vector2d operator+(Vector2d v1, Vector2d v2);
		VECTOR2D_API friend Vector2d operator-(Vector2d v1, Vector2d v2);
		VECTOR2D_API friend bool operator==(const Vector2d v1, const Vector2d v2);
		VECTOR2D_API friend bool operator!=(const Vector2d v1, const Vector2d v2);
		VECTOR2D_API friend double inner_product(const Vector2d v1, const Vector2d v2);
	};
}



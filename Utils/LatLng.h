#ifdef UTILS_EXPORTS
#define LATLNG_API __declspec(dllexport)
#else
#define LATLNG_API __declspec(dllimport)
#endif

#pragma once

namespace Geography 
{

	///<summary>
	/// �ܓx�E�o�x��\���N���X
	///</summary>
	class LATLNG_API LatLng
	{
	private:
		double latitude;
		double longitude;

		void set_lat(double latitude);
		void set_lng(double longitude);

	public:
		LatLng(double latitude, double longitude);
		virtual ~LatLng();
		double lat() const;
		double lng() const;
	};
}
#pragma once
#include "FileExportable.h"
#include "Coordinate.h"
#include "LatLng.h"

namespace Entity 
{

	///<summary>
	/// ユーザ，及びダミーの状態を記述するデータ構造
	/// 座標系によってPOSITION_TYPEを使い分ける(Coordinate or LatLng)
	///</summary>
	template <typename POSITION_TYPE>
	class BasicMobileEntityState : public IO::FileExportable
	{
	
	//Coordinateの派生クラスかチェック
	static_assert(std::is_base_of<Graph::Coordinate, POSITION_TYPE>::value, "template type POSITION_TYPE must be derived from Coordinate");

	protected:
		static const std::string USER_PROBABILITY;

		std::shared_ptr<POSITION_TYPE> position;
		double user_probability;

	public:
		BasicMobileEntityState(const POSITION_TYPE& position, double probability);
		virtual ~BasicMobileEntityState();

		double get_user_probability() const;
		const std::shared_ptr<POSITION_TYPE const> read_position() const;

		std::unordered_map<std::string, std::string> get_export_data() const;
	};

	//明示的特殊化
	template class BasicMobileEntityState<Graph::Coordinate>;
	template class BasicMobileEntityState<Geography::LatLng>;
}


